// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician_FSM.h"
#include "Enemy_Magician.h"
#include "Enemy_Magician_Anim.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include "OnsSoulPlayer.h"

// Sets default values for this component's properties
UEnemy_Magician_FSM::UEnemy_Magician_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempattackMontage(TEXT("AnimMontage'/Game/LJW/Enemys/SimpleSkeleton/anim-m/AM_Enemy_Maizc.AM_Enemy_Maizc'"));
	if (tempattackMontage.Succeeded())
	{
		AttackMontage = tempattackMontage.Object;
	}
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
void UEnemy_Magician_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

	target = Cast<AOnsSoulPlayer>(actor);

	me = Cast<AEnemy_Magician>(GetOwner());

	anim = Cast<UEnemy_Magician_anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	originPos = me->GetActorLocation();

	me->HpWidget->UpdateCurrHP(hp, maxhp);
	
}


// Called every frame
void UEnemy_Magician_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState3::Idle:
		IdleState();
		break;
	case EEnemyState3::Move:
		MoveState();
		break;
	case EEnemyState3::Attack:
		AttackState();
		break;
	case EEnemyState3::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyState3::Run:
		BackRun();
		break;
	case EEnemyState3::Damage:
		DamageState();
		break;
	case EEnemyState3::Die:
		DieState();
		break;
	case EEnemyState3::ReturnPos:
		UpdateReturnPos();
		break;
	}

	if (!(target == nullptr))
	{
		FVector Ddir = target->GetActorLocation() - me->GetActorLocation();
		FVector DdirSize = Ddir;
		Ddir.Normalize();
		float Ddotvalue = FVector::DotProduct(me->GetActorForwardVector(), Ddir);
		float Dangle = UKismetMathLibrary::DegAcos(Ddotvalue);
		//멀어지면 체력바안보이도록 수정
		if (DdirSize.Size() > 2000)
		{
			me->HpWidget->SetVisibility(false);
		}

		if (Dangle > 175 && DdirSize.Size() < 300)
		{
			Hitback = true;
		}
		else
		{
			Hitback = false;
		}
	}
}

void UEnemy_Magician_FSM::IdleState()
{
	
	//시야에 들어오면 움직이기 시작
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState3::Move);
	}

	else
	{
		//idleDelayTime 이 지나면	
		if (IsWaitComplete(idleDelayTime))
		{
			//현재상태를 Move 로 한다.
			ChangeState(EEnemyState3::Move);
		}
	}
}
void UEnemy_Magician_FSM::MoveState()
{
	// 시야에 들어왔는지 여부
	bool bTrace = IsTargetTrace();
	if (!(target == nullptr))
	{
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
			
				/*	if (dir.Length() < 500)
					{
						ChangeState(EEnemyState3::Run);
					}
					else
				{*/
					ChangeState(EEnemyState3::Attack);
				//}
			
			
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
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// 랜덤한 위치까지 도착한 후 Idle 상태로 전환
		MoveToPos(randPos);
	}
}

void UEnemy_Magician_FSM::AttackState()
{
	

	
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime > 3)
	{

		currTime = 0;

		ChangeState(EEnemyState3::AttackDelay);
	}

	
}

void UEnemy_Magician_FSM::BackRun()
{


	anim->animState = mState;

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState3::Attack);
	}


}

void UEnemy_Magician_FSM::UpdaetAttackDelay()
{
	if (IsWaitComplete(attackDelayTime))
	{

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		//if (dist < 1000)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("RUN"));
		//	ChangeState(EEnemyState3::Run);
		//	/*	EEnemyState3::Run;
		//		anim->animState = mState;*/
		//}
		//else 
		if (dist < attackRange)
		{
		
			ChangeState(EEnemyState3::Attack);
		}
		else
		{
			ChangeState(EEnemyState3::Idle);
		}
	}
}
void UEnemy_Magician_FSM::DamageState()
{
	if (Hitback)
	{
		Hitback = false;
	}
	//damageDelayTime 이 지나면
	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState3::Idle);
	}
		
}
void UEnemy_Magician_FSM::DieState()
{
	me->HpWidget->SetVisibility(false);
	//아직 죽음 애니메이션이 끝나지 않았다면
	//바닥내려가지 않도록 처리
	/*if (anim->bDieDone == false)
	{
		return;
	}*/

	//계속 아래로 내려가고 싶다.
	//동속ㅇ운동ㅇ 공식 피=피제+브이티
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	////2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	//if (p.Z < -200)
	//{
		//me->Destroy();
	//}
	////3. 그렇지 않으면 해당 위치로 셋팅한다
	//else
	//{
	//	me->SetActorLocation(p);
	//}


}
void UEnemy_Magician_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}
void UEnemy_Magician_FSM::OnDamageProcess(float damage)
{
	//피격효과음
	UGameplayStatics::PlaySound2D(GetWorld(), HITSound);
	//체력 위젯 보이도록
	me->HpWidget->SetVisibility(true);
	//체력감소
	hp -= damage;
	//hp--;
	//피격되었을때 hp표시줄을 보여주는거 한번만 실행되면됨
	me->HpWidget->UpdateCurrHP(hp, maxhp);

	if (hp > 0)
	{
		if (Hitback)
		{
			UE_LOG(LogTemp, Warning, TEXT("backattack"));

			FString sectionName = FString::Printf(TEXT("BackAttack"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			//뒤로 밀려나는 함수
			FTimerHandle aaa;
			GetWorld()->GetTimerManager().SetTimer(aaa, this, &UEnemy_Magician_FSM::moveBack, 1.0f, false);
			mState = EEnemyState3::Damage;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("defultattack"));
			//피격 애니메이션 재생
			FString sectionName = FString::Printf(TEXT("Damage0"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			FVector imp = target->GetActorForwardVector() * 500.0f;
			me->GetCharacterMovement()->AddImpulse(imp, true);
			mState = EEnemyState3::Damage;
		}

	}
	else
	{
		//상태를 죽음으로 전환
		mState = EEnemyState3::Die;
		//캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//죽음 애니메이션 재생
		FString sectionName = FString::Printf(TEXT("Die"));
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
		GetWorld()->SpawnActor<AActor>(dropFactory, me->GetActorTransform());
		//사망효과음
		UGameplayStatics::PlaySound2D(GetWorld(), SeeplayerSound);

	}
	//애니메이션 상태 동기화
	anim->animState = mState;
	ai->StopMovement();
}



bool UEnemy_Magician_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UEnemy_Magician_FSM::mazic()
{
	if (!(target == nullptr))
	{
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (mazicattack==0)
	{
		//공격투사체 발사
		GetWorld()->SpawnActor<AActor>(MazicFactory, me->GetMesh()->GetSocketLocation(TEXT("Socket")), dirx, params);
		//공격이펙트 생성
		GetWorld()->SpawnActor<AActor>(MazicAttackEffectFactory, me->GetMesh()->GetSocketLocation(TEXT("Socket")), dirx, params);
	}
	else
	{
		//공격 마법진 생성
		GetWorld()->SpawnActor<AActor>(MazicAttackFactory, attackZone, target->GetActorRotation());
	}

	

	ChangeState(EEnemyState3::AttackDelay);
	}
	else
	{
		MoveToPos(originPos);
	}
}

void UEnemy_Magician_FSM::groggy()
{
	ai->StopMovement();
	ChangeState(EEnemyState3::Idle);
}

void UEnemy_Magician_FSM::moveBack()
{
	FVector imp =(target->GetActorForwardVector()) * 2000.0f;
	me->GetCharacterMovement()->AddImpulse(imp, true);
}

bool UEnemy_Magician_FSM::IsTargetTrace()
{
	if (target == nullptr) return false;

	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//구한 값이 40보다 작고 적과 플레이어와의 거리가 지정한 거리보다 작으면
	if (angle < 70 && dirSize.Size() < traceRange)
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

bool UEnemy_Magician_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return false;
}

void UEnemy_Magician_FSM::ChangeState(EEnemyState3 state)
{
	//상태 변경 로그를 출력하자
	/*UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
			*enumPtr->GetNameStringByIndex((int32)mState),
			*enumPtr->GetNameStringByIndex((int32)state));
	}*/

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
	case EEnemyState3::Attack:
		{
		FVector des = target->GetActorLocation();

		FVector dir = des - me->GetActorLocation();

		FRotator dirx = dir.Rotation();

		me->SetActorRotation(dirx);

		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		mazicattack = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		FString mazicName = FString::Printf(TEXT("Attack%d"), mazicattack);
		//3. 몽타주를 플레이한다.
		me->PlayAnimMontage(AttackMontage, 1.0f, FName(*mazicName));

		if (mazicattack==1)
		{
			//플레이어 바닥에 공격을 할거라는 경고표시를 스폰한다
			attackZone=target->GetActorLocation() - FVector(0, 0, 80);
			GetWorld()->SpawnActor<AActor>(MazicReadyFactory,attackZone,target->GetActorRotation());

			//2초후에 바다에서 나오는 불 공격 생성
			FTimerHandle ddd;
			GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_Magician_FSM::mazic, 1.0f, false);
				
			
		}
		else
		{   //1초뒤에 투사체 발사
			FTimerHandle ddd;
			GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_Magician_FSM::mazic, 1.0f, false);
		}
		}
		break;
	case EEnemyState3::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState3::Damage:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState3::Die:

		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	



		//Die 몽타주 실행
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}

void UEnemy_Magician_FSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemyState3::Idle);
	}
}