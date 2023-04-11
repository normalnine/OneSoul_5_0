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
		//�־����� ü�¹پȺ��̵��� ����
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
	if (!(target == nullptr))
	{
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
			
				/*	if (dir.Length() < 500)
					{
						ChangeState(EEnemyState3::Run);
					}
					else
				{*/
					ChangeState(EEnemyState3::Attack);
				//}
			
			
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
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// ������ ��ġ���� ������ �� Idle ���·� ��ȯ
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
	//damageDelayTime �� ������
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
	//���� ���� �ִϸ��̼��� ������ �ʾҴٸ�
	//�ٴڳ������� �ʵ��� ó��
	/*if (anim->bDieDone == false)
	{
		return;
	}*/

	//��� �Ʒ��� �������� �ʹ�.
	//���Ӥ���� ���� ��=����+����Ƽ
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	////2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	//if (p.Z < -200)
	//{
		//me->Destroy();
	//}
	////3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
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
	//�ǰ�ȿ����
	UGameplayStatics::PlaySound2D(GetWorld(), HITSound);
	//ü�� ���� ���̵���
	me->HpWidget->SetVisibility(true);
	//ü�°���
	hp -= damage;
	//hp--;
	//�ǰݵǾ����� hpǥ������ �����ִ°� �ѹ��� ����Ǹ��
	me->HpWidget->UpdateCurrHP(hp, maxhp);

	if (hp > 0)
	{
		if (Hitback)
		{
			UE_LOG(LogTemp, Warning, TEXT("backattack"));

			FString sectionName = FString::Printf(TEXT("BackAttack"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			//�ڷ� �з����� �Լ�
			FTimerHandle aaa;
			GetWorld()->GetTimerManager().SetTimer(aaa, this, &UEnemy_Magician_FSM::moveBack, 1.0f, false);
			mState = EEnemyState3::Damage;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("defultattack"));
			//�ǰ� �ִϸ��̼� ���
			FString sectionName = FString::Printf(TEXT("Damage0"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			FVector imp = target->GetActorForwardVector() * 500.0f;
			me->GetCharacterMovement()->AddImpulse(imp, true);
			mState = EEnemyState3::Damage;
		}

	}
	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState3::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//���� �ִϸ��̼� ���
		FString sectionName = FString::Printf(TEXT("Die"));
		me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
		GetWorld()->SpawnActor<AActor>(dropFactory, me->GetActorTransform());
		//���ȿ����
		UGameplayStatics::PlaySound2D(GetWorld(), SeeplayerSound);

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
	if (!(target == nullptr))
	{
	FVector des = target->GetActorLocation();

	FVector dir = des - me->GetActorLocation();

	FRotator dirx = dir.Rotation();

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (mazicattack==0)
	{
		//��������ü �߻�
		GetWorld()->SpawnActor<AActor>(MazicFactory, me->GetMesh()->GetSocketLocation(TEXT("Socket")), dirx, params);
		//��������Ʈ ����
		GetWorld()->SpawnActor<AActor>(MazicAttackEffectFactory, me->GetMesh()->GetSocketLocation(TEXT("Socket")), dirx, params);
	}
	else
	{
		//���� ������ ����
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
	/*UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
			*enumPtr->GetNameStringByIndex((int32)mState),
			*enumPtr->GetNameStringByIndex((int32)state));
	}*/

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
		FVector des = target->GetActorLocation();

		FVector dir = des - me->GetActorLocation();

		FRotator dirx = dir.Rotation();

		me->SetActorRotation(dirx);

		//1. ������ ���� �̴´� (0, 1 ��)
		mazicattack = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		FString mazicName = FString::Printf(TEXT("Attack%d"), mazicattack);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		me->PlayAnimMontage(AttackMontage, 1.0f, FName(*mazicName));

		if (mazicattack==1)
		{
			//�÷��̾� �ٴڿ� ������ �ҰŶ�� ���ǥ�ø� �����Ѵ�
			attackZone=target->GetActorLocation() - FVector(0, 0, 80);
			GetWorld()->SpawnActor<AActor>(MazicReadyFactory,attackZone,target->GetActorRotation());

			//2���Ŀ� �ٴٿ��� ������ �� ���� ����
			FTimerHandle ddd;
			GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_Magician_FSM::mazic, 1.0f, false);
				
			
		}
		else
		{   //1�ʵڿ� ����ü �߻�
			FTimerHandle ddd;
			GetWorld()->GetTimerManager().SetTimer(ddd, this, &UEnemy_Magician_FSM::mazic, 1.0f, false);
		}
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