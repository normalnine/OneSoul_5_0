// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_FSM.h"
#include "OnsSoulPlayer.h"
#include "Enemy_HpBar.h"
#include "Weapon.h"
#include "AIController.h"
#include "Enemy_HpBar_WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JW_ParryGuardComponent.h"
#include "JW_PlayerBaseComponent.h"
#include "OneSoulPlayerAnimInstance.h"
#include "Enemy_Skeleton_Anim.h"
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Engine/EngineTypes.h"
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/Actor.h>
#include <Components/BoxComponent.h>
#include "Shield.h"



AEnemy_Skeleton::AEnemy_Skeleton()
{
	//���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SKELETON.SKELETON'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//Į�޽� ����
	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwoedMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	SwordMeshComp->SetupAttachment(GetMesh(), "Bip01-R-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSworodMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SWORD_W_sword_01.SWORD_W_sword_01'"));
	if (TempSworodMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		SwordMeshComp->SetStaticMesh(TempSworodMesh.Object);

	}

	//���� �޽� ����
	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	ShieldMeshComp->SetupAttachment(GetMesh(), "Bip01-L-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempShieldMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SHIELD.SHIELD'"));
	if (TempShieldMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		ShieldMeshComp->SetStaticMesh(TempShieldMesh.Object);

	}


	SwordCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollisionComp->SetupAttachment(SwordMeshComp);

	collisionComp =CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldCollision"));
	collisionComp->SetupAttachment(ShieldMeshComp);


	SwordCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Skeleton::OnOverlapBeginsword);

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Skeleton::OnOverlapBeginshield);


	fsm = CreateDefaultSubobject<UEnemy_Skeleton_FSM>(TEXT("FSM"));



	HpWidget = CreateDefaultSubobject<UEnemy_HpBar_WidgetComponent>(TEXT("HpBar"));
	HpWidget->SetupAttachment(GetRootComponent());

	//���� ĸ���� Į�� ������
	UCapsuleComponent* mycapsule = GetCapsuleComponent();
	mycapsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Skeleton::OnOverlapME);

	ConstructorHelpers::FObjectFinder<USoundBase> tempbSound(TEXT("SoundWave'/Game/LJW/Enemys/sound/070_Equip_10.070_Equip_10'"));
	if (tempbSound.Succeeded())
	{
		blockSound = tempbSound.Object;
	}
	SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy_Skeleton::BeginPlay()
{
	Super::BeginPlay();

	HpWidget->UpdateCurrHP(fsm->hp, fsm->maxhp);

	
}

void AEnemy_Skeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fsm->isShield)
	{
	collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemy_Skeleton::OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{

 		AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
 		
 		if (target != nullptr)
 		{
 			if (target->parrying)
 			{
				//FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();
				//while (It)
				//{
				//	APlayerController* Pcon = It->Get();
				//	// ������ �÷��̾� ��Ʈ�ѷ� ��ü�� ó��
				//	++It;
				//	player->EnableInput(Pcon);
				//}
				/*SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);*/
				//ȿ���� �߻�
				UGameplayStatics::PlaySound2D(GetWorld(), blockSound);
				changeGroggy = true;
				FActorSpawnParameters params;
				params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				//����Ʈ �߻�
				GetWorld()->SpawnActor<AActor>(effectfactory1, target->GetMesh()->GetSocketTransform("ring_01_l"), params);
			}
 			else
 			{	
				if(target->Health>1)
				{
				SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//�÷��̾� ������ ȣ���ϴ��Լ�
				UGameplayStatics::ApplyDamage(target, 40, fsm->ai, this, UDamageType::StaticClass());
				//�÷��̾ �˹��Ų��
				FVector imp = -1 * target->GetActorForwardVector() * 500.0f;
				target->GetCharacterMovement()->AddImpulse(imp, true);
				//�÷��̾� ī�޶� ��鸮��
				//player->Shake();
				}
			}
 		
 
 		}
		AShield* shield = Cast<AShield>(otherActor);
		if (shield !=nullptr)
		{
			if (!(fsm->superArm))
			{
			
			FString sectionName = FString::Printf(TEXT("thing"));
			PlayAnimMontage(fsm->damageMontage, 1.0f, FName(*sectionName));
			//Į ���� �浹ü ���
			SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//ȿ���� �߻�
			UGameplayStatics::PlaySound2D(GetWorld(), blockSound);

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			//ƨ������ ����Ʈ ����
			GetWorld()->SpawnActor<AActor>(effectfactory,shield->GetActorTransform(),params);
			
			
			auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

			player = Cast<AOnsSoulPlayer>(actor);

				if (player !=nullptr)
				{
				
					//�÷��̾ �˹��Ų��
					FVector imp = -1 * player->GetActorForwardVector() * 3000.0f;
					player->GetCharacterMovement()->AddImpulse(imp, true);

					//�÷��̾� ī�޶� ��鸮��
					player->Shake();

					//�÷��̾��� ��� ����
					player->CurrentStamina = FMath::Clamp(player->CurrentStamina - 10.f, player->MinStamina, player->MaxStamina);

					//�÷��̾��� ��� ȸ�� ���ʵڿ� ȣ���ؾ��ϴµ�
					FTimerHandle ddd;
					GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Skeleton::reStamina, 2.0f, false);
				}
			}
			
		}
}

void AEnemy_Skeleton::OnOverlapBeginshield(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWeapon* weapon = Cast<AWeapon>(otherActor);
	if (weapon != nullptr)
	{
		//Į�ݸ������
		SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//ƨ������ ����Ʈ ����
		GetWorld()->SpawnActor<AActor>(effectfactory,weapon->GetActorTransform(), params);
		
		//�ǰ�ȣ��ȵǵ��� ���� ����
		one=true;
		
		//���п� ������ ���� �ݸ����� ��� ��� �ٽ� Ű�°� - ���� �°� ���¾Ƽ� ����� ������ �ȵǴϱ�
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		//ȿ���� �߻�
		UGameplayStatics::PlaySound2D(GetWorld(), blockSound);
	
		//��Ÿ�ֽ���
		FString sectionName = FString::Printf(TEXT("Block"));
		PlayAnimMontage(fsm->damageMontage, 1.0f, FName(*sectionName));
		//������Ʈ ����
		fsm->mState = EEnemyState1::Shield;
		//�÷��̾� �ҷ�����
		auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());
		player = Cast<AOnsSoulPlayer>(actor);
		//�÷��̾� ī�޶� ��鸮��
		player->Shake();
		//�÷��̾ ���и� ������ �ݵ� �ִϸ��̼� ���
		player->compPlayerBase->SAR();
		//�÷��̾� Į �ݸ��� ���
		weapon->WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//�÷��̾� �ڷ� �˹��Ű��
		FVector imp = player->GetActorForwardVector() * -3000.0f;
		player->GetCharacterMovement()->AddImpulse(imp, true);
		//���ݾȵǴ� �����־ �ٽ� �����Ϸ���
		player->IsAttacking=false;
		

		//1�� �ڿ� ���� �ݸ����� Ű�� �Լ�ȣ��
		FTimerHandle ddd;
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Skeleton::oncoll, 1.5f, false);
	}
}

void AEnemy_Skeleton::reStamina()
{
	player->SprintTimer();
	player->RegenerateStamina();
}

void AEnemy_Skeleton::OnOverlapME(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWeapon* weapon = Cast<AWeapon>(otherActor);
	if (weapon != nullptr)
	{
		SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//GetWorld()->SpawnActor<AActor>(HitresultFactory, GetActorTransform(), params);

		//FVector HitLocation = SweepResult.Location;
		/*FName HitLocation = SweepResult.BoneName;
		GetWorld()->SpawnActor<AActor>(HitresultFactory, GetMesh()->GetSocketLocation(TEXT("*HitLocation")),weapon->GetActorRotation()-GetActorRotation(),params);*/
		
		fsm->OnDamageProcess(weapon->randDamage);
		/*	FString LocationString = HitLocation.ToString();`

			UKismetSystemLibrary::PrintString(this, LocationString, true, true, FLinearColor::Green); */
	
	}
}

void AEnemy_Skeleton::oncoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	one=false;
}
