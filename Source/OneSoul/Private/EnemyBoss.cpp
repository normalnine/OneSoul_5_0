// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "EnemyBossFSM.h"
#include "EnemyBossAnim.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SphereComponent.h>
#include "OnsSoulPlayer.h"
#include <Components/AudioComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include <GameFramework/DamageType.h>
#include <Kismet/GameplayStatics.h>
#include <AIModule/Classes/AIController.h>


// Sets default values
AEnemyBoss::AEnemyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeScale3D(FVector(2.0));

	capsuleCompHead = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Head Collision"));
	capsuleCompHead->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MOUNTAIN_DRAGON_-HeadSocket"));
	capsuleCompHead->SetRelativeRotation(FRotator(0, 30, 90));
	capsuleCompHead->SetCapsuleSize(70.0f, 150.0f);
	capsuleCompHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	sphereCompHand_L = CreateDefaultSubobject<USphereComponent>(TEXT("Left Hand Collision"));
	sphereCompHand_L->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MOUNTAIN_DRAGON_-L-HandSocket"));
	sphereCompHand_L->SetRelativeLocation(FVector(50,0,0));
	sphereCompHand_L->SetSphereRadius(80.0f);
	sphereCompHand_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	sphereCompHand_R = CreateDefaultSubobject<USphereComponent>(TEXT("Right Hand Collision"));
	sphereCompHand_R->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MOUNTAIN_DRAGON_-R-HandSocket"));
	sphereCompHand_R->SetRelativeLocation(FVector(50, 0, 0));
	sphereCompHand_R->SetSphereRadius(80.0f);
	sphereCompHand_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	capsuleCompBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body Collision"));
	capsuleCompBody->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("MOUNTAIN_DRAGON_-SpineSocket"));
	
	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	audioComp->SetupAttachment(RootComponent);

	particleComp1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System1"));
	particleComp1->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/QuadrapedCreatures/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
	if (tempMesh.Succeeded())
	{

		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	fsm = CreateDefaultSubobject<UEnemyBossFSM>(TEXT("FSM"));

	// �ִϸ��̼� �������Ʈ �Ҵ��ϱ�
	ConstructorHelpers::FClassFinder<UEnemyBossAnim> tempClass(TEXT("AnimBlueprint'/Game/KDH/Animation/ABP_MountainDragon.ABP_MountainDragon_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	// ���忡 ��ġ�ǰų� ������ �� �ڵ�����
	// AIController ���� Process �� �� �ֵ��� ����
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


}

// Called when the game starts or when spawned
void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	capsuleCompHead->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss::BeginOverlapHead);
	//capsuleCompHead->OnComponentEndOverlap.AddDynamic(this, &AEnemyBoss::EndOverlapHead);

	sphereCompHand_L->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss::BeginOverlapHead);
	//sphereCompHand_L->OnComponentEndOverlap.AddDynamic(this, &AEnemyBoss::EndOverlapHead);

	sphereCompHand_R->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBoss::BeginOverlapHead);
	//sphereCompHand_R->OnComponentEndOverlap.AddDynamic(this, &AEnemyBoss::EndOverlapHead);

	audioComp->Play();
}

// Called every frame
void AEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AEnemyBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// float AEnemyBoss::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
// {
// 	if (fsm->currHP - Damage <= 0.f)
// 	{
// 		fsm-> currHP = 0.f;
// 	}
// 	else
// 	{
// 		fsm-> ReceiveDamage(Damage);
// 	}
// 
// 	return Damage;
// }

void AEnemyBoss::SetActive(bool bActive)
{
	// Ȱ��ȭ
	if (bActive)
	{
		//�浹 Ȱ��
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//���� ��ġ �缳��
		fsm->originPos = GetActorLocation();
	}
	// ��Ȱ��ȭ
	else
	{
		//�浹 ��Ȱ��
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//źâ�� �� �ٽ� �־���
		//dieDelegate.ExecuteIfBound(this);
	}

	//�޽��� Ȱ�� / ��Ȱ��
	GetMesh()->SetActive(bActive);
	//�޽��� ���̰� / �Ⱥ��̰�
	GetMesh()->SetVisibility(bActive);
	//ĳ���� �����Ʈ Ȱ�� / ��Ȱ��
	GetCharacterMovement()->SetActive(bActive);
	//fsm Ȱ�� / ��Ȱ��
	fsm->SetActive(bActive);
}

void AEnemyBoss::BeginOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// �÷��̾� ĳ���� & ������ �Լ� ȣ��
		target = Cast<AOnsSoulPlayer>(OtherActor);
		if (target != nullptr && target->Health > 0)
		{
			UGameplayStatics::ApplyDamage(
				target,
				100,
				fsm->ai,
				this,
				UDamageType::StaticClass());
			fsm->Roar(2000.0f);
		}
		bOverlap = false;
	}
}

void AEnemyBoss::BeginOverlapLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// �÷��̾� ĳ���� & ������ �Լ� ȣ��
		target = Cast<AOnsSoulPlayer>(OtherActor);
		if (target != nullptr && target->Health > 0)
		{
			UGameplayStatics::ApplyDamage(
				target,
				100,
				fsm->ai,
				this,
				UDamageType::StaticClass());
			fsm->Roar(2000.0f);

		}
		bOverlap = false;
	}
}

void AEnemyBoss::BeginOverlapRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// �÷��̾� ĳ���� & ������ �Լ� ȣ��
		target = Cast<AOnsSoulPlayer>(OtherActor);
		if (target != nullptr && target->Health > 0)
		{
			UGameplayStatics::ApplyDamage(
				target,
				100,
				fsm->ai,
				this,
				UDamageType::StaticClass());
			fsm->Roar(2000.0f);
		}
		bOverlap = false;
	}
}


// void AEnemyBoss::EndOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	bOverlap = true;
// }
// 
// void AEnemyBoss::EndOverlapLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	bOverlap = true;
// }
// 
// void AEnemyBoss::EndOverlapRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// 	bOverlap = true;
// }
