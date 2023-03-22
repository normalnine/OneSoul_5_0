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

	//�þ߿� ������ �����̱� ����
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState5::Move);
	}

	else
	{
		//idleDelayTime �� ������	
		if (IsWaitComplete(idleDelayTime))
		{
			//������¸� Move �� �Ѵ�.
			ChangeState(EEnemyState5::Move);
		}
	}
}
void UEnemy_Archer_FSM::MoveState()
{

	//UE_LOG(LogTemp, Warning, TEXT("moVE"));
	// �þ߿� ���Դ��� ����
	bool bTrace = IsTargetTrace();

	//Ÿ���� ���ϴ� ������ ���ϰ�
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//ó�� ��ġ, ���� ���� ��ġ�� �Ÿ�
	float dist = FVector::Distance(originPos, me->GetActorLocation());


	//�þ߿� ���Դٸ�
	if (bTrace)
	{
		//UE_LOG(LogTemp, Warning, TEXT("trace"));
		//���࿡ target - me �Ÿ��� ���ݹ������� ������
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
		//�׷��� ������
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("movetotarget"));
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//�þ߿� ������ �ʾҴٸ�
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// ������ ��ġ���� ������ �� Idle ���·� ��ȯ
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
	//����÷��̾� ������ �ٶ󺸵���

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
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		ChangeState(EEnemyState5::Idle);
	}
}
void UEnemy_Archer_FSM::DieState()
{


	////���Ӥ���� ���� ��=����+����Ƽ
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	////2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	//if (p.Z < -200)
	//{
	//	me->Destroy();
	//}
	////3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
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
	//ü�°���
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
			//�ǰ� �ִϸ��̼� ���
			FString sectionName = FString::Printf(TEXT("Damage0"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));

			mState = EEnemyState5::Damage;
		}

	}
	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState5::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//���� �ִϸ��̼� ���
		FString sectionName = FString::Printf(TEXT("Die"));
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));


	}
	//�ִϸ��̼� ���� ����ȭ
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

	//�Ѿ���  ���� ���� �����Ѵ�
	GetWorld()->SpawnActor<AActor>(ArrowFactory, me->GetMesh()->GetSocketLocation(TEXT("Socket2")), dirx, params);
	ChangeState(EEnemyState5::AttackDelay);
}

void UEnemy_Archer_FSM::groggy()
{
	ai->StopMovement();

	//�ִϸ��̼� �Ǵ� ��Ÿ�� �� ����

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
	//���� ���� 70���� �۰� ���� �÷��̾���� �Ÿ��� ������ �Ÿ����� ������
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
	//���� ���¸� ����
	mState = state;

	//anim �� ���� ����
	anim->animState = state;

	//���� �ð� �ʱ�ȭ
	currTime = 0;

	//ai �� ������ ������
	ai->StopMovement();

	//���¿� ���� �ʱ⼳��
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
		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState5::Damage:
	{
		//1. ������ ���� �̴´� (0, 1 ��)
		//int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		//FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState5::Die:

		//�浹�ȵǰ� ����
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);




		//Die ��Ÿ�� ����
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_Archer_FSM::ReceiveDamage()
{
	//�Ǹ� ������
	hp--;
	//hp �� 0���� ũ�� Damage ���·� ��ȯ
	if (hp > 0)
	{
		ChangeState(EEnemyState5::Damage);
	}
	//�׷��� ������ Die ���·� ��ȯ
	else
	{

		ChangeState(EEnemyState5::Die);
	}
}

void UEnemy_Archer_FSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
		ChangeState(EEnemyState5::Idle);
	}
}
