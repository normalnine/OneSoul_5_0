// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"
#include "EnemyBossFSM.h"
#include "EnemyBossAnim.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SphereComponent.h>

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


	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/QuadrapedCreatures/MountainDragon/Meshes/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
	if (tempMesh.Succeeded())
	{

		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	fsm = CreateDefaultSubobject<UEnemyBossFSM>(TEXT("FSM"));

	// ?????????? ?????????? ????????
	ConstructorHelpers::FClassFinder<UEnemyBossAnim> tempClass(TEXT("AnimBlueprint'/Game/KDH/Animation/ABP_MountainDragon.ABP_MountainDragon_C'"));
	if (tempClass.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	// ?????? ?????????? ?????? ?? ????????
	// AIController ???? Process ?? ?? ?????? ????
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

void AEnemyBoss::SetActive(bool bActive)
{
	// ??????
	if (bActive)
	{
		//???? ????
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		//???? ???? ??????
		fsm->originPos = GetActorLocation();
	}
	// ????????
	else
	{
		//???? ??????
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//?????? ?? ???? ??????
		//dieDelegate.ExecuteIfBound(this);
	}

	//?????? ???? / ??????
	GetMesh()->SetActive(bActive);
	//?????? ?????? / ????????
	GetMesh()->SetVisibility(bActive);
	//?????? ???????? ???? / ??????
	GetCharacterMovement()->SetActive(bActive);
	//fsm ???? / ??????
	fsm->SetActive(bActive);
}

void AEnemyBoss::BeginOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// ???????? ?????? & ?????? ???? ????
		bOverlap = false;
	}
}

void AEnemyBoss::BeginOverlapLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// ???????? ?????? & ?????? ???? ????
		bOverlap = false;
	}
}

void AEnemyBoss::BeginOverlapRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// ???????? ?????? & ?????? ???? ????
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
