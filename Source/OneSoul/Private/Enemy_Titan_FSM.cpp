// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Titan_FSM.h"
#include "Enemy_Titan.h"
#include "Enemy_Titan_anim.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include "OnsSoulPlayer.h"
#include <GameFramework/Character.h>
// Sets default values for this component's properties
UEnemy_Titan_FSM::UEnemy_Titan_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/LJW/Enemys/Titan/anim/AM_Enemy_Titan.AM_Enemy_Titan'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempHITSound(TEXT("SoundWave'/Game/LJW/Enemys/sound/zombie10.zombie10'"));
	if (tempHITSound.Succeeded())
	{
		HITSound = tempHITSound.Object;
	}
}


// Called when the game starts
void UEnemy_Titan_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

	target = Cast<AOnsSoulPlayer>(actor);

	me = Cast<AEnemy_Titan>(GetOwner());

	anim = Cast<UEnemy_Titan_anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	originPos = me->GetActorLocation();

	me->HpWidget->UpdateCurrHP(hp, maxhp);
	
}


// Called every frame
void UEnemy_Titan_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState4::Idle:
		IdleState();
		break;
	case EEnemyState4::Move:
		MoveState();
		break;
	case EEnemyState4::MovetoTarget:
		movetoPlayer();
		break;
	case EEnemyState4::Attack0:
		AttackState();
		break;
	case EEnemyState4::Attack1:
		AttackState1();
		break;
	case EEnemyState4::Attack2:
		AttackState2();
		break;
	case EEnemyState4::Attack3:
		AttackState3();
		break;
	case EEnemyState4::JumpAttack:
		jumpattack();
		break;
	case EEnemyState4::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyState4::Damage:
		DamageState();
		break;
	case EEnemyState4::Die:
		DieState();
		break;
	case EEnemyState4::ReturnPos:
		UpdateReturnPos();
		break;
	case EEnemyState4::Groggy:
		UpdateReturnPos();
		break;
	}

	FVector Ddir = target->GetActorLocation() - me->GetActorLocation();
	FVector DdirSize = Ddir;
	Ddir.Normalize();
	float Ddotvalue = FVector::DotProduct(me->GetActorLocation(), Ddir);
	float Dangle = UKismetMathLibrary::DegAcos(Ddotvalue);
	//멀어지면 체력바안보이도록 수정
	if (DdirSize.Size() > 2000)
	{
		me->HpWidget->SetVisibility(false);
	}
	//뒷각인지 확인하는거
	if (Dangle > 155 && DdirSize.Size() < 500)
	{
		Hitback = true;
	}
	else
	{
		Hitback = false;
	}
	//플레이어 카메라 흔들리는 효과
	if (camShake == true)
	{
		//2. 현재시간을 흐르게 하고
		currCamShakeTime += DeltaTime;
		//3. 만약에 현재시간이 기준시간보다 작으면
		if (currCamShakeTime < camShakeTime)
		{
			//4. 카메라를 랜덤하게 위치시키자
			float randY = FMath::RandRange(-5.0f, 5.0f);
			float randZ = FMath::RandRange(-5.0f, 5.0f);
			target->Camera->SetRelativeLocation(FVector(0, randY, randZ));
		}
		//5. 그렇지 않으면 초기화(현재시간, bFire, 카메라위치)
		else
		{
			currCamShakeTime = 0;
			camShake = false;
			target->Camera->SetRelativeLocation(FVector::ZeroVector);
		}
	}
}
void UEnemy_Titan_FSM::IdleState()
{
	
	superArmor=false;

	//시야에 들어오면 움직이기 시작
	if (IsTargetTrace())
	{	//UE_LOG(LogTemp, Warning, TEXT("I_SEE"));
		if (jumptotarget)
		{
			//UE_LOG(LogTemp, Warning, TEXT("JUMPTOYOU"));
			ChangeState(EEnemyState4::JumpAttack);
			jumptotarget=false;
			
			
		}
		//ChangeState(EEnemyState4::Move);
	}

	//else
	//{
	//	//idleDelayTime 이 지나면	
	//	if (IsWaitComplete(idleDelayTime))
	//	{
	//		//현재상태를 Move 로 한다.
	//		ChangeState(EEnemyState4::Move);
	//	}
	//}
}
void UEnemy_Titan_FSM::MoveState()
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
		
		//만약에 target - me 거리가 공격범위보다 작으면
		if (dir.Length() < attackRange)
		{
			
			int32 index = FMath::RandRange(0, 1);
			if (index == 0)
			{
				ChangeState(EEnemyState4::Attack0);
			}
			else
			{
				ChangeState(EEnemyState4::Attack1);
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

void UEnemy_Titan_FSM::movetoPlayer()
{
	ai->MoveToLocation(target->GetActorLocation());

	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	//만약에 target - me 거리가 공격범위보다 작으면
	if (dir.Length() < attackRange)
	{

		int32 index = FMath::RandRange(0, 1);
		if (index == 0)
		{
			ChangeState(EEnemyState4::Attack0);
		}
		else
		{
			ChangeState(EEnemyState4::Attack1);
		}
	}
}

void UEnemy_Titan_FSM::AttackState()
{
	//몬스터가 플레이어 방향으로 공격하도록 하는 거 안함
	FVector des = target->GetActorLocation();
	FVector dir = des - me->GetActorLocation();
	FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//애니메이션에서 끄고 키는걸로 수정
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	if (me->changeGroggy)
	{
		me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ChangeState(EEnemyState4::Groggy);
	}
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}
}

void UEnemy_Titan_FSM::AttackState1()
{
	superArmor = true;
	//몬스터가 플레이어 방향으로 공격하도록 하는 거
	//FVector des = target->GetActorLocation();
	//FVector dir = des - me->GetActorLocation();
	//FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//애니메이션에서 끄고 키는걸로 수정
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}


}
void UEnemy_Titan_FSM::AttackState2()
{
	superArmor=true;

	//몬스터가 플레이어 방향으로 공격하도록 하는 거
	//FVector des = target->GetActorLocation();
	//FVector dir = des - me->GetActorLocation();
	//FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//애니메이션에서 끄고 키는걸로 수정
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}


}
void UEnemy_Titan_FSM::AttackState3()
{
	superArmor = true;

	//몬스터가 플레이어 방향으로 공격하도록 하는 거
	//FVector des = target->GetActorLocation();
	//FVector dir = des - me->GetActorLocation();
	//FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//애니메이션에서 끄고 키는걸로 수정
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}


}

void UEnemy_Titan_FSM::jumpattack()
{
	// 이동하는 루틴 호출
	ai->MoveToLocation(movetarget);

	// 점프 공격
	float JumpHeight = 500.0f;
	float JumpTime = FMath::Sqrt(2 * JumpHeight / GetWorld()->GetGravityZ());
	float JumpSpeed = JumpHeight / JumpTime;
	FVector JumpVelocity = FVector(0, 0, JumpSpeed);
	me->GetCharacterMovement()->Launch(JumpVelocity);
	currTime += GetWorld()->DeltaTimeSeconds;
	if (stopmove)
	{
	ai->StopMovement();
		if (currTime > 4)
		{
			currTime = 0;
			ChangeState(EEnemyState4::MovetoTarget);
		}
	}
	
}

void UEnemy_Titan_FSM::UpdaetAttackDelay()
{	
	superArmor=true;
	//팔의 충돌 콜리전 끄는거
	me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (IsWaitComplete(attackDelayTime))
	{
		
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < attackRange)
		{
			//기본 공격 패턴 2개
			int32 index = FMath::RandRange(0, 1);
			//hp가 적어지면 공격패턴 추가
			if (hp<11)
			{index =FMath::RandRange(0,2);}

			//공격패턴 실행
			if (index == 0)
			{ChangeState(EEnemyState4::Attack1);}
			else if(index ==1)
			{ChangeState(EEnemyState4::Attack2);}
			else if(index ==2)
			{ChangeState(EEnemyState4::Attack3);}
			
		}
		else
		{

			ChangeState(EEnemyState4::Move);
		}
	}
}
void UEnemy_Titan_FSM::DamageState()
{
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemyState4::Move);
	}
}
void UEnemy_Titan_FSM::DieState()
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
	//	me->Destroy();
	//}
	//3. 그렇지 않으면 해당 위치로 셋팅한다
	//else
	//{
	//	me->SetActorLocation(p);
	//}


}
void UEnemy_Titan_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}
void UEnemy_Titan_FSM::OnDamageProcess()
{
	me->HpWidget->SetVisibility(true);
	hp--;
	me->HpWidget->UpdateCurrHP(hp, maxhp);
	
		//체력이 남아있다면
		if (hp > 0)
		{

			//체력이 절반이면 2페이즈 진입
			if (hp<10 && shout == true)
			{	
				UE_LOG(LogTemp, Warning, TEXT("2page"));
				//몽타주실행
				FString sectionName = FString::Printf(TEXT("Shout"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				//슈퍼아머상태로 변경
				superArmor=true;
				//한번만 실행하려고 폴스로 바꿈
				shout=false;
				mState = EEnemyState4::AttackDelay;
			}
			//뒷각에서 맞았을때
			if (Hitback&&!superArmor)
			{
				//몽타주실행
				FString sectionName = FString::Printf(TEXT("Cri"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				//상태변경
				mState = EEnemyState4::Damage;
			}
			//슈퍼아머 상태가 아닐때, 즉 일반 피격모션
			if (!(superArmor))
			{
				//효과음재생
				UGameplayStatics::PlaySound2D(GetWorld(), HITSound);
				FString sectionName = FString::Printf(TEXT("Damage0"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				FVector imp = target->GetActorForwardVector() * 500.0f;
				me->GetCharacterMovement()->AddImpulse(imp, true);
			//상태를 피격으로 전환
			mState = EEnemyState4::Damage;
			currentTime = 0;
			}
		}

		else
		{
			//상태를 죽음으로 전환
			mState = EEnemyState4::Die;
			//캡슐 충돌체 비활성화
			me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//죽음 애니메이션 재생
			FString sectionName = FString::Printf(TEXT("Die"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			GetWorld()->SpawnActor<AActor>(dropFactory, me->GetActorTransform());
		}
		//애니메이션 상태 동기화
		anim->animState = mState;
		ai->StopMovement();
	
		
}



bool UEnemy_Titan_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UEnemy_Titan_FSM::groggy()
{
	UE_LOG(LogTemp, Warning, TEXT("groggy"));
	anim->animState = mState;

	me->changeGroggy = false;

	ai->StopMovement();
	
	ChangeState(EEnemyState4::Idle);

}

void UEnemy_Titan_FSM::Shake()
{
		camShake = true;
}

bool UEnemy_Titan_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//구한 값이 40보다 작고 적과 플레이어와의 거리가 지정한 거리보다 작으면
	if (angle < 90 && dirSize.Size() < traceRange)
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

bool UEnemy_Titan_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return false;
}

void UEnemy_Titan_FSM::ChangeState(EEnemyState4 state)
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
	case EEnemyState4::Attack0:
		//currTime = attackDelayTime;
		break;
	case EEnemyState4::Move:
	{	
		superArmor=false;

		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;

	case EEnemyState4::JumpAttack:
	{
		movetarget = target->GetActorLocation();
		/*FTimerHandle aaa;
		GetWorld()->GetTimerManager().SetTimer(aaa,this,&UEnemy_Titan_FSM::Shake,2,false);*/
	}
	break;

	case EEnemyState4::Damage:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState4::Die:

		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);




		//Die 몽타주 실행
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_Titan_FSM::ReceiveDamage()
{
	//피를 줄이자
	hp--;
	//hp 가 0보다 크면 Damage 상태로 전환
	if (hp > 0)
	{
		ChangeState(EEnemyState4::Damage);
	}
	//그렇지 않으면 Die 상태로 전환
	else
	{

		ChangeState(EEnemyState4::Die);
	}
}

void UEnemy_Titan_FSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemyState4::Idle);
	}
}
