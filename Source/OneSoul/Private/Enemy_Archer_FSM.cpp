// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Archer_FSM.h"
#include "Enemy_Archer.h"
#include "Enemy_Archer_anim.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include "OnsSoulPlayer.h"
// Sets default values for this component's properties
UEnemy_Archer_FSM::UEnemy_Archer_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/LJW/Enemys/SimpleSkeleton/anim/AM_Enemy_Skeleton.AM_Enemy_Skeleton'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
	}
}


// Called when the game starts
void UEnemy_Archer_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

	target = Cast<AOnsSoulPlayer>(actor);

	me = Cast<AEnemy_Archer>(GetOwner());

	anim = Cast<UEnemy_Archer_anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	originPos = me->GetActorLocation();
	
}


// Called every frame
void UEnemy_Archer_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState5::Idle:
		IdleState();
		break;
	case EEnemyState5::Move:
		MoveState();
		break;
	case EEnemyState5::Attack:
		AttackState();
		break;
	case EEnemyState5::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyState5::Run:
		BackRun();
		break;
	case EEnemyState5::Damage:
		DamageState();
		break;
	case EEnemyState5::Die:
		DieState();
		break;
	case EEnemyState5::ReturnPos:
		UpdateReturnPos();
		break;
	}
}

void UEnemy_Archer_FSM::IdleState()
{
	anim->animState = mState;

	//시야에 들어오면 움직이기 시작
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState5::Move);
	}

	else
	{
		//idleDelayTime 이 지나면	
		if (IsWaitComplete(idleDelayTime))
		{
			//현재상태를 Move 로 한다.
			ChangeState(EEnemyState5::Move);
		}
	}
}
void UEnemy_Archer_FSM::MoveState()
{

	//UE_LOG(LogTemp, Warning, TEXT("moVE"));
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

			if (dir.Length() < 500)
			{
				ChangeState(EEnemyState5::Run);
			}
			else
			{
				ChangeState(EEnemyState5::Attack);
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

void UEnemy_Archer_FSM::AttackState()
{
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	me->SetActorRotation(dirx);
	ChangeState(EEnemyState5::AttackDelay);

}

void UEnemy_Archer_FSM::BackRun()
{


	anim->animState = mState;

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState5::Attack);
	}


}

void UEnemy_Archer_FSM::UpdaetAttackDelay()
{
	//계속플레이어 방향을 바라보도록

	FVector dira = target->GetActorLocation() - me->GetActorLocation();
	FRotator dirx = dira.Rotation();
	me->SetActorRotation(dirx);


	if (IsWaitComplete(attackDelayTime))
	{

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < 500)
		{
			EEnemyState5::Run;
			anim->animState = mState;
		}
		else if (dist < attackRange)
		{

			ChangeState(EEnemyState5::Attack);
		}
		else
		{
			ChangeState(EEnemyState5::Idle);
		}
	}
}
void UEnemy_Archer_FSM::DamageState()
{
	//damageDelayTime 이 지나면
	if (IsWaitComplete(damageDelayTime))
	{
		//Move 상태
		ChangeState(EEnemyState5::Idle);
	}
}
void UEnemy_Archer_FSM::DieState()
{


	////동속ㅇ운동ㅇ 공식 피=피제+브이티
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	////2. 만약에 p.Z 가 -200 보다 작으면 파괴한다
	//if (p.Z < -200)
	//{
	//	me->Destroy();
	//}
	////3. 그렇지 않으면 해당 위치로 셋팅한다
	//else
	//{
	//	me->SetActorLocation(p);
	//}


}
void UEnemy_Archer_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}
void UEnemy_Archer_FSM::OnDamageProcess()
{

	UE_LOG(LogTemp, Warning, TEXT("IMHIT"));
	//체력감소
	//hp -= damage;
	hp--;
	if (hp > 0)
	{
		if (Hitback)
		{
			UE_LOG(LogTemp, Warning, TEXT("backattack"));

			FString sectionName = FString::Printf(TEXT("BackAttack"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			mState = EEnemyState5::Damage;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("defultattack"));
			//피격 애니메이션 재생
			FString sectionName = FString::Printf(TEXT("Damage0"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));

			mState = EEnemyState5::Damage;
		}

	}
	else
	{
		//상태를 죽음으로 전환
		mState = EEnemyState5::Die;
		//캡슐 충돌체 비활성화
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//죽음 애니메이션 재생
		FString sectionName = FString::Printf(TEXT("Die"));
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));


	}
	//애니메이션 상태 동기화
	anim->animState = mState;
	ai->StopMovement();




}


bool UEnemy_Archer_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UEnemy_Archer_FSM::mazic()
{
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//총알을  소켓 에서 스폰한다
	GetWorld()->SpawnActor<AActor>(ArrowFactory, me->GetMesh()->GetSocketLocation(TEXT("Socket2")), dirx, params);
	ChangeState(EEnemyState5::AttackDelay);
}

void UEnemy_Archer_FSM::groggy()
{
	ai->StopMovement();

	//애니메이션 또는 몽타주 를 실행

	ChangeState(EEnemyState5::Idle);
}

void UEnemy_Archer_FSM::moveBack()
{

}

bool UEnemy_Archer_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);
	//UE_LOG(LogTemp, Warning, TEXT("%f"),angle);
	//구한 값이 70보다 작고 적과 플레이어와의 거리가 지정한 거리보다 작으면
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

bool UEnemy_Archer_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return false;
}

void UEnemy_Archer_FSM::ChangeState(EEnemyState5 state)
{
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
	case EEnemyState5::Attack:
	{

		FTimerHandle ddd;
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_Archer_FSM::mazic, 0.1f, false);


	}
	break;
	case EEnemyState5::Move:
	{
		//네비게이션 시스템 가져오자
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//랜덤한 위치를 얻오자
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState5::Damage:
	{
		//1. 랜덤한 값을 뽑는다 (0, 1 중)
		//int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 이란 문자열을 만든다.
		//FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. 몽타주를 플레이한다.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState5::Die:

		//충돌안되게 설정
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);




		//Die 몽타주 실행
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_Archer_FSM::ReceiveDamage()
{
	//피를 줄이자
	hp--;
	//hp 가 0보다 크면 Damage 상태로 전환
	if (hp > 0)
	{
		ChangeState(EEnemyState5::Damage);
	}
	//그렇지 않으면 Die 상태로 전환
	else
	{

		ChangeState(EEnemyState5::Die);
	}
}

void UEnemy_Archer_FSM::MoveToPos(FVector pos)
{
	//해당 위치(pos) 로 간다
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//만약에 목적지에 도착했다면
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle 상태로 전환
		ChangeState(EEnemyState5::Idle);
	}
}
