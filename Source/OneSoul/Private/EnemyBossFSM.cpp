// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossFSM.h"
#include <Kismet/GameplayStatics.h>
//#include "OneSoulCharacter.h"
#include "EnemyBoss.h"
#include "EnemyBossAnim.h"
#include <AIModule/Classes/AIController.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/CapsuleComponent.h>
#include "EnemyBossFireball.h"
#include "EnemyBossFireSpread.h"
#include "OnsSoulPlayer.h"
#include "EnemyBossHPBar.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "EnemyBossLaser.h"
#include "EnemyBossGhost.h"
#include "EnemyBossDieUI.h"
#include "OneSoulGameInstance.h"
#include <Materials/MaterialInterface.h>
#include <Components/AudioComponent.h>
#include <Particles/ParticleSystemComponent.h>


// Sets default values for this component's properties
UEnemyBossFSM::UEnemyBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UEnemyBossFSM::BeginPlay()
{
	Super::BeginPlay();

	//Ÿ�� ã��
	target = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));	//���� ã��
	
	me = Cast<AEnemyBoss>(GetOwner());

	//anim instance ã��
	/*USkeletalMeshComponent* mesh = me->GetMesh();
	UAnimInstance animInstance = mesh->GetAnimInstance();
	anim = Cast<UEnemyAnim>(animInstance);*/
	anim = Cast<UEnemyBossAnim>(me->GetMesh()->GetAnimInstance());

	//ai controller ã��
	ai = Cast<AAIController>(me->GetController());
	//ai = UAIBlueprintHelperLibrary::GetAIController(me);

	//���� �ʱ� ü���� ��������
	currHP = maxHP;

	//���� �ʱ� ��ġ�� ��������
	originPos = me->GetActorLocation();
}


// Called every frame
void UEnemyBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (currState)
	{
	case EEnemyBossState::Idle:
		UpdateIdle();
		break;
	case EEnemyBossState::Move:
		UpdateMove();
		break;
	case EEnemyBossState::Attack:
		UpdateAttack();
		break;
	case EEnemyBossState::AttackDelay:
		UpdaetAttackDelay();	
		break;
	case EEnemyBossState::Damaged:
		UpdateDamaged();
		break;
	case EEnemyBossState::Die:
		UpdateDie();
		break;
// 	case EEnemyBossState::ReturnPos:
// 		UpdateReturnPos();
// 		break;
	}
}

void UEnemyBossFSM::UpdateIdle()
{
	//���࿡ �÷��̾ �Ѿ� �� �� �ִ�?
	if (IsTargetTrace())
	{
		//���¸� Move �� ��ȯ
		ChangeState(EEnemyBossState::Move);
	}
	else
	{
		//idleDelayTime �� ������	
		if (IsWaitComplete(idleDelayTime))
		{
			//������¸� Move �� �Ѵ�.
			ChangeState(EEnemyBossState::Move);
		}
	}
}

void UEnemyBossFSM::UpdateMove()
{
	// �þ߿� ���Դ��� ����
	bool bTrace = IsTargetTrace();

	//1. Ÿ���� ���ϴ� ������ ���ϰ�(target - me)
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

// 	//ó�� ��ġ, ���� ���� ��ġ�� �Ÿ�
// 	float dist = FVector::Distance(originPos, me->GetActorLocation());
// 
// 	//���࿡ dist �� moveRange ���� Ŀ���� (������ �� �ִ� �ݰ��� �Ѿ�ٸ�)
// // 	if (dist > moveRange)
// // 	{
// // 		//���¸� ReturnPos �� ����
// // 		UE_LOG(LogTemp, Warning, TEXT("ReturnPos"));
// // 		ChangeState(EEnemyBossState::ReturnPos);
// // 	}
// 	//���࿡ �þ߿� ���Դٸ�
// 	else if (bTrace)
	if (bTrace)
	{
		//UE_LOG(LogTemp, Warning, TEXT("bTrace"));
// 		if (dist < avoidRange)
// 		{
// 			MoveToPos(randPos);
// 			UE_LOG(LogTemp, Warning, TEXT("randPos"));
// 		}
		//���࿡ target - me �Ÿ��� ���ݹ������� ������
		if (dir.Length() < attackRange)
		{
			//���¸� Attack ���� ����
			ChangeState(EEnemyBossState::AttackDelay);
		}
		//�׷��� ������
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("AddMovementInput"));

			//2. �� �������� �̵��ϰ� �ʹ�.
			//me->AddMovementInput(dir.GetSafeNormal());
			//me->SetActorRotation()
			//ai �� �̿��ؼ� ���������� �̵��ϰ� �ʹ�.	
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//�þ߿� ������ �ʾҴٸ�
	else
	{
		
		// ������ ��ġ���� ������ �� Idle ���·� ��ȯ
		//UE_LOG(LogTemp, Warning, TEXT("randPos"));
		MoveToPos(randPos);
	}

}

void UEnemyBossFSM::UpdateAttack()
{
	//2. ���¸� AttackDelay �� ��ȯ	
	ChangeState(EEnemyBossState::AttackDelay);
}

void UEnemyBossFSM::UpdaetAttackDelay()
{
	//2. ���࿡ ����ð��� attackDelayTime ���� Ŀ����
	if (IsWaitComplete(attackDelayTime))
	{
		//3. target �� me �Ÿ��� ������.
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		//4. ���࿡ �װŸ��� attackRange���� ������
		if (dist < attackRange)
		{
			//5. Attack ���·� ��ȯ
			DecideAttackPattern();
			ChangeState(EEnemyBossState::Attack);
		}
		else
		{
			//6. �׷��� ������ Idle ���·� ����
			ChangeState(EEnemyBossState::Idle);
		}
	}
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	dir.Normalize();
	FRotator targetRotation = FRotationMatrix::MakeFromX(dir).Rotator();
	FRotator NewRotation = FMath::RInterpTo(me->GetActorRotation(), targetRotation, GetWorld()->DeltaTimeSeconds, 1.0f);
	me->SetActorRotation(NewRotation);
}

void UEnemyBossFSM::UpdateDamaged()
{
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Damage0")));
		//anim->Montage_Stop(5.0f, damageMontage);

		ChangeState(EEnemyBossState::Idle);
	}
}

void UEnemyBossFSM::UpdateDie()
{
	//���࿡ bDieMove �� false ��� �Լ��� ������
	//if (bDieMove == false) return;

	UMaterialInstanceDynamic* CharacterMaterial = me->GetMesh()->CreateDynamicMaterialInstance(0);
	if (CharacterMaterial)
	{
		//UE_LOG(LogTemp, Error, TEXT("CharacterMaterial"));
		CurrentOpacity-=0.002f;

		CharacterMaterial->SetScalarParameterValue(TEXT("Opacity"), CurrentOpacity);
		me->GetMesh()->SetMaterial(0, CharacterMaterial);
	}

	if (CurrentOpacity < 0)
	{
		gameInst->npcState = ENPCState::Complete;
		me->Destroy();
	}	
}

void UEnemyBossFSM::UpdateReturnPos()
{
	// ó�� ��ġ�� ���� �����ϸ� Idle ���·� ��ȯ�Ѵ�.
	MoveToPos(originPos);


	////1. �� ----> ó����ġ�� ���ϴ� ������ ���Ѵ�.
	//FVector dir = originPos - me->GetActorLocation();

	////2. ���࿡ �� --- ó����ġ�� �Ÿ��� 10���� ������
	//if (dir.Length() < 10)
	//{
	//	//3. Idle ���·� ��ȯ
	//	ChangeState(EEnemyState::Idle);
	//}
	////4. �׷��� ������ 
	//else
	//{
	//	//5. ��� 1������ ���� �������� �̵��Ѵ�
	//	me->AddMovementInput(dir.GetSafeNormal());
	//}
}

void UEnemyBossFSM::ChangeState(EEnemyBossState state)
{
	//���� ���� �α׸� �������
// 	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
// 	if (enumPtr != nullptr)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
// 			*enumPtr->GetNameStringByIndex((int32)currState),
// 			*enumPtr->GetNameStringByIndex((int32)state));
// 	}

	//���� ���¸� ����
	currState = state;

	//anim �� ���� ����
	anim->state = state;

	//���� �ð� �ʱ�ȭ
	currTime = 0;

	//ai �� ������ ������
	ai->StopMovement();

	//���¿� ���� �ʱ⼳��
	switch (currState)
	{
	case EEnemyBossState::Attack:
	{
		//currTime = attackDelayTime;
	}
	break;
	case EEnemyBossState::Move:
	{
		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000.0f, loc);
		randPos = loc.Location;
	}
	break;
	case EEnemyBossState::Damaged:
	{
		//1. ������ ���� �̴´� (0, 1 ��)
		//int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		//FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Damage0")));
		accumulatedDamage = 0;
		bGroggy = true;
		FTimerHandle WaitHandle;
		float WaitTime = 2.0f; //�ð��� �����ϰ�
		GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
			{
				if (currHP > 0)
				{
					me->StopAnimMontage(damageMontage);
					bGroggy = false;
					ChangeState(EEnemyBossState::Idle);
				}
			}), WaitTime, false);
	}
	break;
	case EEnemyBossState::Die:
		//�浹�ȵǰ� ����
		me->capsuleCompBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		//Die ��Ÿ�� ����
		me->particleComp1->SetVisibility(true);
		me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));
		me->audioComp->Stop();
		
		
		EnemyBossHPBar->RemoveFromParent();
		EnemyBossDieUI = CreateWidget<UEnemyBossDieUI>(GetWorld(), EnemyBossDieUIFactory);
		if (EnemyBossDieUI != nullptr)
		{
			EnemyBossDieUI->AddToViewport();
		}
		
		if (gameInst == nullptr)
		{
			gameInst = Cast<UOneSoulGameInstance>(GetWorld()->GetGameInstance());
			gameInst->soul += soul;
		}

		UMaterialInterface* Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/QuadrapedCreatures/MountainDragon/Materials/M_MOUNTAIN_DRAGON_2.M_MOUNTAIN_DRAGON_2'")));
		if (Material)
		{
			me->GetMesh()->SetMaterial(0, Material);
		}		
		break;
	}
}

void UEnemyBossFSM::ReceiveDamage(float damage)
{
	//�Ǹ� ������
	currHP -= damage;
	EnemyBossHPBar->UpdateCurrHP(currHP, maxHP);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), slashedSound, me->GetActorLocation());
	FTimerHandle WaitHandle;
	float WaitTime = 1.0f; //�ð��� �����ϰ�
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			EnemyBossHPBar->UpdateBackCurrHP(currHP, maxHP);

		}), WaitTime, false);

	//�������� ��������
	if(!bGroggy) accumulatedDamage += damage;
	EnemyBossHPBar->UpdateAccumulatedDamage(accumulatedDamage);

	bDamageDealtRecently = true;
	GetWorld()->GetTimerManager().SetTimer(DamageResetTimerHandle, this, &UEnemyBossFSM::ResetDamageIfNoRecentDamage, 2.0f, false);

	if (currHP > 0)
	{
		if (accumulatedDamage > groggyDamage && !bGroggy)
		{
			ChangeState(EEnemyBossState::Damaged);
		}
	}
	else
	{
		ChangeState(EEnemyBossState::Die);
	}
}

bool UEnemyBossFSM::IsWaitComplete(float delayTime)
{
	//1. �ð��� �帣�� �Ѵ�.
	currTime += GetWorld()->DeltaTimeSeconds;

	//2. ���࿡ ����ð��� delayTime���� Ŀ���� 
	if (currTime > delayTime)
	{
		//3. ����ð��� 0���� �ʱ�ȭ
		currTime = 0;
		//4. true ��ȯ
		return true;
	}

	//5. �׷��� ������ false �� ��ȯ
	return false;
}


bool UEnemyBossFSM::IsTargetTrace()
{
	//UE_LOG(LogTemp, Warning, TEXT("IsTargetTrace"));

	//1. �� ----> �÷��̾ ���ϴ� ����
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//2. ���� �չ���� 1���� ���� ������ ����
	float dotValue = FVector::DotProduct(me->GetActorForwardVector(), dir.GetSafeNormal());

	//3. 2���� ���� ���� Acos --> �� ������ ���̰�
	float angle = UKismetMathLibrary::DegAcos(dotValue);

	//4. ���࿡ 3������ ���� ���� 30���� �۰�(�þ߰� 60)
	//   �� - Ÿ�� ���� �Ÿ��� traceRange ���� ������
	//if (angle < 60 && dir.Length() < traceRange)
	if (dir.Length() < traceRange)
	{
		if (EnemyBossHPBar == nullptr)
		{
			EnemyBossHPBar = CreateWidget<UEnemyBossHPBar>(GetWorld(), EnemyBossHPBarFactory);
			if (EnemyBossHPBar != nullptr)
			{

				EnemyBossHPBar->AddToViewport();
				EnemyBossHPBar->UpdateCurrHP(currHP, maxHP);
				EnemyBossHPBar->UpdateBackCurrHP(currHP, maxHP);

			}

		}

		//Enemy -----> target LineTrace ����!!
		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);
		//DrawDebugLine(GetWorld(), me->GetActorLocation(), target->GetActorLocation(), FColor::Cyan);
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			hitInfo,
			me->GetActorLocation(),
			target->GetActorLocation(),
			ECollisionChannel::ECC_Visibility,
			param);

		//���࿡ �ε��� ���� �ִٸ�
		if (bHit)
		{
			//���࿡ �ε��� ���� �̸��� Player �� �����ϰ� �ִٸ�
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")))
			{
				//5. true ��ȯ
				return true;
			}
		}
	}
	//6. �׷��� ������ false ��ȯ
	return false;
}

void UEnemyBossFSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
		ChangeState(EEnemyBossState::Idle);
	}
}

void UEnemyBossFSM::DecideAttackPattern()
{
	InitAttackPattern();
	int32 randPattern = 0;
	if (currHP / maxHP < 0.5f)
	{
		randPattern = FMath::RandRange(0, 8);
		//UE_LOG(LogTemp, Warning, TEXT("2pase"));
	}
	else
	{
		randPattern = FMath::RandRange(0, 8);
		//UE_LOG(LogTemp, Warning, TEXT("1pase"));

	}
	
	//UE_LOG(LogTemp, Warning, TEXT("attackNum : %d"), randPattern);
	//anim->bAttackPlay8 = true;
	switch (randPattern)
	{
	case 0:
		anim->bAttackPlay0 = true;
		break;
	case 1:
		anim->bAttackPlay1 = true;
		break;
	case 2:
		anim->bAttackPlay2 = true;
		break;
	case 3:
		anim->bAttackPlay3 = true;
		break;
	case 4:
		anim->bAttackPlay4 = true;
		break;
	case 5:
		anim->bAttackPlay5 = true;
		break;
	case 6:
		anim->bAttackPlay6 = true;
		break;
	case 7:
		anim->bAttackPlay7 = true;
		break;
	case 8:
		anim->bAttackPlay8 = true;
		break;
	default:
		break;
	}
}

void UEnemyBossFSM::InitAttackPattern()
{
	anim->bAttackPlay0 = false;
	anim->bAttackPlay1 = false;
	anim->bAttackPlay2 = false;
	anim->bAttackPlay3 = false;
	anim->bAttackPlay4 = false;
	anim->bAttackPlay5 = false;
	anim->bAttackPlay6 = false;
	anim->bAttackPlay7 = false;
	anim->bAttackPlay8 = false;
}

void UEnemyBossFSM::SpawnFireball()
{
	if(bGroggy || bDieMove) return;
	AEnemyBossFireball* fireball = GetWorld()->SpawnActor<AEnemyBossFireball>(FireballFactory, me->GetMesh()->GetSocketTransform(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));
}

void UEnemyBossFSM::SpawnFireSpread()
{
	if(bGroggy || bDieMove) return;
	AEnemyBossFireSpread* fireSpread = GetWorld()->SpawnActor<AEnemyBossFireSpread>(FireSpreadFactory, me->GetMesh()->GetSocketTransform(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));	
}

void UEnemyBossFSM::SpawnLaser()
{
	if(bGroggy || bDieMove) return;
	AEnemyBossLaser* Laser = GetWorld()->SpawnActor<AEnemyBossLaser>(LaserFactory, me->GetMesh()->GetSocketTransform(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));
}

void UEnemyBossFSM::ResetDamageIfNoRecentDamage()
{
	if (bDamageDealtRecently)
	{
		bDamageDealtRecently = false;
		GetWorld()->GetTimerManager().SetTimer(DamageResetTimerHandle, this, &UEnemyBossFSM::ResetDamageIfNoRecentDamage, 2.0f, false);
	}
	else
	{
		accumulatedDamage = 0;
		EnemyBossHPBar->UpdateAccumulatedDamage(accumulatedDamage);
	}
}

void UEnemyBossFSM::SpawnGhost()
{
	if(bGroggy) return;

	FTransform trans = me->GetActorTransform();

	
	float WaitTime = 0.05f; //�ð��� �����ϰ�
	GetWorld()->GetTimerManager().SetTimer(GhostHandle, FTimerDelegate::CreateLambda([&]()
		{			
			FVector OffsetVector = FVector::CrossProduct(me->GetActorRotation().Vector(), FVector::UpVector).GetSafeNormal() * ghostDistance;

			ghost = GetWorld()->SpawnActor<AEnemyBossGhost>(GhostFactory, me->GetActorLocation() + OffsetVector, me->GetActorRotation());
			ghost = GetWorld()->SpawnActor<AEnemyBossGhost>(GhostFactory, me->GetActorLocation() - OffsetVector, me->GetActorRotation());

			ghostDistance += 150;

			if (ghostDistance > 1950)
			{
				GetWorld()->GetTimerManager().ClearTimer(GhostHandle);
				ghostDistance = 450;
			}
			
		}), WaitTime, true);
}

void UEnemyBossFSM::Roar(float enemyAttackForce)
{
	UCharacterMovementComponent* PlayerMovementComponent = target->FindComponentByClass<UCharacterMovementComponent>();

	FVector ForceDirection = target->GetActorLocation() - me->GetActorLocation();
	ForceDirection.Normalize();

	PlayerMovementComponent->AddImpulse(ForceDirection * enemyAttackForce, true);
}

void UEnemyBossFSM::CameraShake()
{
	target->camShakeTime = 2.0f;
	target->randC = 2.0f;
	target->randD = 2.0f;
	target->Shake();
}
