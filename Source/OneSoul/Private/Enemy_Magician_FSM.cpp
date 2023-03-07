// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician_FSM.h"
#include "Enemy_Magician.h"
#include "Enemy_Magician_Anim.h"
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

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/LJW/Enemys/SimpleSkeleton/anim-m/AM_Enemy_Maizc.AM_Enemy_Maizc'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
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
}

void UEnemy_Magician_FSM::IdleState()
{
	
	//�þ߿� ������ �����̱� ����
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState3::Move);
	}

	else
	{
		//idleDelayTime �� ������	
		if (IsWaitComplete(idleDelayTime))
		{
			//������¸� Move �� �Ѵ�.
			ChangeState(EEnemyState3::Move);
		}
	}
}
void UEnemy_Magician_FSM::MoveState()
{
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
				ChangeState(EEnemyState3::Run);
			}
			else
			{
				ChangeState(EEnemyState3::Attack);
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

void UEnemy_Magician_FSM::AttackState()
{
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	me->SetActorRotation(dirx);
	ChangeState(EEnemyState3::AttackDelay);	
	
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

		if (dist < 500)
		{
			EEnemyState3::Run;
			anim->animState = mState;
		}
		else if(dist < attackRange)
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
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		ChangeState(EEnemyState3::Idle);
	}
}
void UEnemy_Magician_FSM::DieState()
{

	//���� ���� �ִϸ��̼��� ������ �ʾҴٸ�
	//�ٴڳ������� �ʵ��� ó��
	/*if (anim->bDieDone == false)
	{
		return;
	}*/

	//��� �Ʒ��� �������� �ʹ�.
	//���Ӥ���� ���� ��=����+����Ƽ
	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	//2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	if (p.Z < -200)
	{
		me->Destroy();
	}
	//3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
	else
	{
		me->SetActorLocation(p);
	}


}
void UEnemy_Magician_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}
void UEnemy_Magician_FSM::OnDamageProcess()
{



	
		//ü�°���
		//hp -= damage;
		hp--;
		//ü���� �����ִٸ�
		if (hp > 0)
		{
			//���¸� �ǰ����� ��ȯ
			mState = EEnemyState3::Damage;
			currentTime = 0;
			//�ǰ� �ִϸ��̼� ���
			FString sectionName = FString::Printf(TEXT("Damage0"));
			anim->PlayDamageAnim(FName(*sectionName));


		}


		else
		{
			//���¸� �������� ��ȯ
			mState = EEnemyState3::Die;
			//ĸ�� �浹ü ��Ȱ��ȭ
			me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//���� �ִϸ��̼� ���
			anim->PlayDamageAnim(TEXT("Die"));


		}
		//�ִϸ��̼� ���� ����ȭ
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
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//�Ѿ���  ���� ���� �����Ѵ�
	GetWorld()->SpawnActor<AActor>(MazicFactory, me->GetMesh()->GetSocketLocation(TEXT("Socket")), dirx, params);
	ChangeState(EEnemyState3::AttackDelay);
}

bool UEnemy_Magician_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//���� ���� 40���� �۰� ���� �÷��̾���� �Ÿ��� ������ �Ÿ����� ������
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
	//���� ���� �α׸� �������
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	//if (enumPtr != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
	//		*enumPtr->GetNameStringByIndex((int32)mState),
	//		*enumPtr->GetNameStringByIndex((int32)state));
	//}

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
	case EEnemyState3::Attack:
		{
		//1. ������ ���� �̴´� (0, 1 ��)
		int32 mazic = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		FString mazicName = FString::Printf(TEXT("Attack%d"), mazic);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*mazicName));




		FTimerHandle ddd;
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_Magician_FSM::mazic, 1.0f, false);


		}
		break;
	case EEnemyState3::Move:
	{
		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState3::Damage:
	{
		//1. ������ ���� �̴´� (0, 1 ��)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState3::Die:

		//�浹�ȵǰ� ����
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);




		//Die ��Ÿ�� ����
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_Magician_FSM::ReceiveDamage()
{
	//�Ǹ� ������
	hp--;
	//hp �� 0���� ũ�� Damage ���·� ��ȯ
	if (hp > 0)
	{
		ChangeState(EEnemyState3::Damage);
	}
	//�׷��� ������ Die ���·� ��ȯ
	else
	{

		ChangeState(EEnemyState3::Die);
	}
}

void UEnemy_Magician_FSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
		ChangeState(EEnemyState3::Idle);
	}
}