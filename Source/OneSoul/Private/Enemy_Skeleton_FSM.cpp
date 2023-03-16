// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton_FSM.h"
#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_Anim.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include "OnsSoulPlayer.h"


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
}

void UEnemy_Skeleton_FSM::IdleState()
{
	isShield = false;
	//�þ߿� ������ �����̱� ����
	if (IsTargetTrace())
	{
		ChangeState(EEnemyState1::Move);
	}

	else
	{
		//idleDelayTime �� ������	
		if (IsWaitComplete(idleDelayTime))
		{
			//������¸� Move �� �Ѵ�.
			ChangeState(EEnemyState1::Move);
		}
	}
}
void UEnemy_Skeleton_FSM::MoveState()
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
			if (fight)
			{
				index = FMath::RandRange(0, 1);
			}
			
			if (index == 0)
			{
				ChangeState(EEnemyState1::Attack);
			}
			else
			{
				ChangeState(EEnemyState1::Shield);
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

void UEnemy_Skeleton_FSM::AttackState()
{
	me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	anim->animState = mState;

	currTime += GetWorld()->DeltaTimeSeconds;
	if (me->changeGroggy)
	{	me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ChangeState(EEnemyState1::Groggy);
	}
	if (currTime > 2)
	{

		currTime = 0;
		me->SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	if (IsWaitComplete(attackDelayTime))
	{

		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < attackRange)
		{

			ChangeState(EEnemyState1::Attack);
		}
		else
		{

			ChangeState(EEnemyState1::Move);
		}
	}
}
void UEnemy_Skeleton_FSM::DamageState()
{	
	
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		ChangeState(EEnemyState1::Attack);
	}
}
void UEnemy_Skeleton_FSM::DieState()
{

	//��� �Ʒ��� �������� �ʹ�.
	//���Ӥ���� ���� ��=����+����Ƽ
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	//2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	/*if (p.Z < -200)*/
	//{
		me->Destroy();
	//}
	////3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
	//else
	//{
	//	me->SetActorLocation(p);
	//}


}
void UEnemy_Skeleton_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}



void UEnemy_Skeleton_FSM::OnDamageProcess()
{
	

	if (isShield)
	{
		UE_LOG(LogTemp, Warning, TEXT("imGuard"));
		FString sectionName = FString::Printf(TEXT("Block"));
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
		mState = EEnemyState1::Shield;
	}
	else
	{

		//��������� �������� Ȯ���ϴ� ��
		FVector Ddir = target->GetActorLocation() - me->GetActorLocation();
		FVector DdirSize = Ddir;
		Ddir.Normalize();
		float Ddotvalue = FVector::DotProduct(me->GetActorLocation(), Ddir);
		float Dangle = UKismetMathLibrary::DegAcos(Ddotvalue);

		//ü�°���
		//hp -= damage;
		hp--;
		if (hp > 0)
		{
			if (cri)
			{
				UE_LOG(LogTemp, Warning, TEXT("criattack"));
				
				FString sectionName = FString::Printf(TEXT("Cri"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				mState = EEnemyState1::Damage;
			}
			else if (Dangle>155 && DdirSize.Size() < 500)
			{
				
			

				UE_LOG(LogTemp, Warning, TEXT("backattack%f"),Dangle);
			
				FString sectionName = FString::Printf(TEXT("BackAttack"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				mState = EEnemyState1::Damage;
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("defultattack"));
				
			
				currentTime = 0;
				//�ǰ� �ִϸ��̼� ���
				FString sectionName = FString::Printf(TEXT("Damage0"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				
				mState = EEnemyState1::Damage;
			}

		}
		else
		{
			//���¸� �������� ��ȯ
			mState = EEnemyState1::Die;
			//ĸ�� �浹ü ��Ȱ��ȭ
			me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//���� �ִϸ��̼� ���
			anim->PlayDamageAnim(TEXT("Die"));


		}
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		ai->StopMovement();
	}




}


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

	if (currTime > 3)
	{

		currTime = 0;
		cri = false;
		ChangeState(EEnemyState1::Move);
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

	//���� ���� 40���� �۰� ���� �÷��̾���� �Ÿ��� ������ �Ÿ����� ������
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
	case EEnemyState1::Attack:
		{//�����Ҷ� �ѹ��� �÷��̾ Ȯ���ؼ� �÷��̾� �������� ����
		//FVector des = target->GetActorLocation();
		//FVector dir = des - me->GetActorLocation();
		//FRotator dirx = dir.Rotation();
		//me->SetActorRotation(dirx);
		}
		break;
	case EEnemyState1::Move:
	{
		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyState1::Damage:
	{
		//1. ������ ���� �̴´� (0, 1 ��)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState1::Die:

		//�浹�ȵǰ� ����
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);




		//Die ��Ÿ�� ����
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_Skeleton_FSM::ReceiveDamage()
{
	//�Ǹ� ������
	hp--;
	//hp �� 0���� ũ�� Damage ���·� ��ȯ
	if (hp > 0)
	{
		ChangeState(EEnemyState1::Damage);
	}
	//�׷��� ������ Die ���·� ��ȯ
	else
	{

		ChangeState(EEnemyState1::Die);
	}
}

void UEnemy_Skeleton_FSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
		ChangeState(EEnemyState1::Idle);
	}
}