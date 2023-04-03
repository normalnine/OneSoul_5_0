// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton_FSM.h"
#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_Anim.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include "OnsSoulPlayer.h"
#include "../OneSoulGameMode.h"
#include <Components/SphereComponent.h>


// Sets default values for this component's properties
UEnemy_Skeleton_FSM::UEnemy_Skeleton_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/LJW/Enemys/SimpleSkeleton/anim/AM_Enemy_Skeleton.AM_Enemy_Skeleton'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/LJW/Enemys/sound/Demon_20.Demon_20'"));
	if (tempSound.Succeeded())
	{
		SeeplayerSound = tempSound.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempHITSound(TEXT("SoundWave'/Game/LJW/Enemys/sound/zombie10.zombie10'"));
	if (tempHITSound.Succeeded())
	{
		HITSound = tempHITSound.Object;
	}
}


// Called when the game starts
void UEnemy_Skeleton_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

	target = Cast<AOnsSoulPlayer>(actor);

	me = Cast<AEnemy_Skeleton>(GetOwner());

	anim = Cast<UEnemy_Skeleton_Anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	originPos = me->GetActorLocation();

	me->HpWidget->UpdateCurrHP(hp, maxhp);


}


// Called every frame
void UEnemy_Skeleton_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState1::Idle:
		IdleState();
		break;
	case EEnemyState1::Move:
		MoveState();
		break;
	case EEnemyState1::Attack:
		AttackState();
		break;
	case EEnemyState1::Attackcombo1:
		AttackCombo1();
		break;
	case EEnemyState1::Attackcombo2:
		AttackCombo2();
		break;
	case EEnemyState1::Attackcombo3:
		AttackCombo3();
		break;
	case EEnemyState1::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyState1::Shield:
		BlockAttack();
		break;
	case EEnemyState1::Damage:
		DamageState();
		break;
	case EEnemyState1::Die:
		DieState();
		break;
	case EEnemyState1::ReturnPos:
		UpdateReturnPos();
		break;
	case EEnemyState1::Groggy:
		groggy();
		break;
	}

	FVector Ddir = target->GetActorLocation() - me->GetActorLocation();
	FVector DdirSize = Ddir;
	Ddir.Normalize();
	//멀어지면 체력바안보이도록 수정
	if (DdirSize.Size() > 2000)
	{
		me->HpWidget->SetVisibility(false);
	}
	float Ddotvalue = FVector::DotProduct(me->GetActorLocation(), Ddir);
	float Dangle = UKismetMathLibrary::DegAcos(Ddotvalue);
	if (Dangle > 155 && DdirSize.Size() < 500)
	{
		Hitback = true;
	}
	else
	{
		Hitback = false;
	}

}

void UEnemy_Skeleton_FSM::IdleState()
{
	isShield = false;
	//시야에 들어오면 움직이기 시작
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState1::Move);
	}

	else
	{
		//idleDelayTime 이 지나면	
		if (IsWaitComplete(idleDelayTime))
		{
			//현재상태를 Move 로 한다.
			ChangeState(EEnemyState1::Move);
		}
	}
}
void UEnemy_Skeleton_FSM::MoveState()
{
	// 시야에 들어왔는지 여부
	bool bTrace = IsTargetTrace();

	//타겟을 향하는 방향을 구하고
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//처음 위치, 나의 현재 위치의 거리
	float dist = FVector::Distance(originPos, me->GetActorLocation());


	//시야에 들어왔다면
	if (bTrace)
	{
		//UE_LOG(LogTemp, Warning, TEXT("trace"));
		//만약에 target - me 거리가 공격범위보다 작으면
		if (dir.Length() < attackRange)
		{
		//	if (fight)
			//{
				index = FMath::RandRange(0, 2);
			//}
			
			if (index == 0)
			{
				ChangeState(EEnemyState1::Attack);
			}
			else if (index ==1)
			{
				ChangeState(EEnemyState1::Attackcombo1);
			}
			else
			{
				ChangeState(EEnemyState1::Shield);
			}
			
				
			
		}
		//그렇지 않으면
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("movetotarget"));
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//시야에 들어오지 않았다면
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// 랜덤한 위치까지 도착한 후 Idle 상태로 전환
		MoveToPos(randPos);
		
	}
}

void UEnemy_Skeleton_FSM::AttackState()
{
	me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	anim->animState = mState;

	currTime += GetWorld()->DeltaTimeSeconds;
	if (me->changeGroggy)
	{	me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ChangeState(EEnemyState1::Groggy);
	}
	if (currTime > 1.3f)
	{

		currTime = 0;
		me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ChangeState(EEnemyState1::AttackDelay);
	}
	
}

void UEnemy_Skeleton_FSM::AttackCombo1()
{
	superArm = true;
	me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	currTime += GetWorld()->DeltaTimeSeconds;

		
		if (currTime > 2)
		{

			currTime = 0;

			ChangeState(EEnemyState1::AttackDelay);
		}

}

void UEnemy_Skeleton_FSM::AttackCombo2()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime > 1)
	{

		currTime = 0;

		ChangeState(EEnemyState1::Attackcombo3);
	}
	
}

void UEnemy_Skeleton_FSM::AttackCombo3()
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime > 1)
	{

		currTime = 0;

		ChangeState(EEnemyState1::AttackDelay);
	}
	
}

void UEnemy_Skeleton_FSM::BlockAttack()
{
	isShield = true;

	anim->animState = mState;

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState1::Idle);
	}


}

void UEnemy_Skeleton_FSM::UpdaetAttackDelay()
{
	superArm = false;
	if (IsWaitComplete(attackDelayTime))
	{

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < attackRange)
		{

			index = FMath::RandRange(0, 2);
			if (index == 0)
			{
				ChangeState(EEnemyState1::Attack);
			}
			else if (index == 1)
			{
				ChangeState(EEnemyState1::Attackcombo1);
			}
			else
			{
				ChangeState(EEnemyState1::Shield);
			}
		}
		else
		{

			ChangeState(EEnemyState1::Move);
		}
	}
}
void UEnemy_Skeleton_FSM::DamageState()
{	
	if (cri)
	{
		cri = false;
		moveBack();
	}
	if (Hitback)
	{
		Hitback = false;
	}
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		me->SetActorEnableCollision(ECollisionEnabled::QueryOnly);
		
		ChangeState(EEnemyState1::Attack);
	}
}
void UEnemy_Skeleton_FSM::DieState()
{
	me->HpWidget->SetVisibility(false);



}
void UEnemy_Skeleton_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}



void UEnemy_Skeleton_FSM::OnDamageProcess()
{
	//칼,방패 충돌체 끄기
	me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->HpWidget->SetVisibility(true);
	UGameplayStatics::PlaySound2D(GetWorld(), HITSound);
	//if (isShield)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("imGuard"));
	//	FString sectionName = FString::Printf(TEXT("Block"));
	//	me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	//	mState = EEnemyState1::Shield;
	//}
	//else
	//{
		//체력감소
		//hp -= damage;
		hp--;
		//피격되었을때 hp표시줄을 보여주는거 한번만 실행되면됨
		me->HpWidget->UpdateCurrHP(hp,maxhp);
		
		if (hp > 0)
		{	if (!(superArm))
			{
		
				if (cri)
				{	
				/*	FB = true;*/
					FString sectionName = FString::Printf(TEXT("Cri"));
					me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
					FTimerHandle aaa;
					GetWorld()->GetTimerManager().SetTimer(aaa, this, &UEnemy_Skeleton_FSM::moveBack, 1.0f, false);
					mState = EEnemyState1::Damage;
				}
				else if (Hitback)
				{
					/*FB = false;*/
					/*FRotator addRotator = me->GetActorRotation();
					addRotator.Yaw += 90.0f;
					me->SetActorRotation(addRotator);*/
					FString sectionName = FString::Printf(TEXT("BackAttack"));
					me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
					FTimerHandle aaa;
					GetWorld()->GetTimerManager().SetTimer(aaa, this, &UEnemy_Skeleton_FSM::moveBack, 1.0f, false);
					mState = EEnemyState1::Damage;
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("defultattack"));
				
			
					currentTime = 0;
					//피격 애니메이션 재생
					FString sectionName = FString::Printf(TEXT("Damage0"));
					me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				
					mState = EEnemyState1::Damage;
				}
			}

		}
		else
		{
			//상태를 죽음으로 전환
			mState = EEnemyState1::Die;
			//캡슐 충돌체 비활성화
			me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//죽음 애니메이션 재생
			FString sectionName = FString::Printf(TEXT("Die"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			GetWorld()->SpawnActor<AActor>(dropFactory, me->GetActorTransform());
			//사망효과음
			UGameplayStatics::PlaySound2D(GetWorld(), SeeplayerSound);

			AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();
			AOneSoulGameMode* CurrentGameModeBase = Cast<AOneSoulGameMode>(CurrentMode);
			if (CurrentGameModeBase != nullptr)
			{
				CurrentGameModeBase->AddCoins(10);
			}
			
		}
		//애니메이션 상태 동기화
		anim->animState = mState;
		ai->StopMovement();
	}




//}


bool UEnemy_Skeleton_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UEnemy_Skeleton_FSM::groggy()
{
	cri = true;
	anim->animState = mState;

	currTime += GetWorld()->DeltaTimeSeconds;

	me->changeGroggy=false;
	if (currTime>2)
	{
		cri = false;
	}

	if (currTime > 3)
	{

		currTime = 0;
		
		ChangeState(EEnemyState1::Move);
	}

}

void UEnemy_Skeleton_FSM::moveBack()
{

		//뒤로살짝밀려가게
		/*int32 a;
		if (FB){a=-1;}else{a=1;}*/
		FVector imp = /*a * */(target->GetActorForwardVector()) * 5000.0f;
		me->GetCharacterMovement()->AddImpulse(imp, true);
	

}

void UEnemy_Skeleton_FSM::showHP()
{
	if (me->HpWidget)
	{
		me->HpWidget->SetVisibility(true);
	}
	
}

void UEnemy_Skeleton_FSM::gof()
{
	if (timerCount<3)
	{	
		timerCount++;
		FVector imp = me->GetActorForwardVector() * 1500.0f;
		me->GetCharacterMovement()->AddImpulse(imp, true);
		if (timerCount == 3)
		{
			GetWorld()->GetTimerManager().ClearTimer(ddd);
		}
	}
}

bool UEnemy_Skeleton_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//구한 값이 40보다 작고 적과 플레이어와의 거리가 지정한 거리보다 작으면
	if (angle < 50 && dirSize.Size() < traceRange)
	{


		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);
		bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, me->GetActorLocation(), target->GetActorLocation(), ECollisionChannel::ECC_EngineTraceChannel4, param);

		if (bhit)
		{
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")))
			{
				

				return true;
			}

		}

	}

	return false;

}

bool UEnemy_Skeleton_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return false;
}

void UEnemy_Skeleton_FSM::ChangeState(EEnemyState1 state)
{
	//상태 변경 로그를 출력하자
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	//if (enumPtr != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
	//		*enumPtr->GetNameStringByIndex((int32)mState),
	//		*enumPtr->GetNameStringByIndex((int32)state));
	//}

	//현재 상태를 갱신
	mState = state;

	//anim 의 상태 갱신
	anim->animState = state;

	//현재 시간 초기화
	currTime = 0;

	//ai 의 움직임 멈추자
	ai->StopMovement();

	//상태에 따른 초기설정
	switch (mState)
	{
	case EEnemyState1::Attack:
		{
		}
		break;
	case EEnemyState1::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState1::Damage:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState1::Attackcombo1:
	{
		
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_Skeleton_FSM::gof, 1.0f, false);
	}
	break;
	case EEnemyState1::Die:

		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	



		//Die 몽타주 실행
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_Skeleton_FSM::ReceiveDamage()
{
	//피를 줄이자
	hp--;
	//hp 가 0보다 크면 Damage 상태로 전환
	if (hp > 0)
	{
		ChangeState(EEnemyState1::Damage);
	}
	//그렇지 않으면 Die 상태로 전환
	else
	{

		ChangeState(EEnemyState1::Die);
	}
}

void UEnemy_Skeleton_FSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		
		//Idle 상태로 전환
		ChangeState(EEnemyState1::Idle);
	}
}

