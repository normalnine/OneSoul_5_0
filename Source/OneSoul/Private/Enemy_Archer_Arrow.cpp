// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Archer_Arrow.h"
#include <Components/SphereComponent.h>
#include "OnsSoulPlayer.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemy_Archer_Arrow::AEnemy_Archer_Arrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));


	RootComponent = collisionComp;


	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));

	bodyMeshComp->SetupAttachment(collisionComp);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	movementComp->SetUpdatedComponent(collisionComp);

	movementComp->InitialSpeed = 600;

	movementComp->MaxSpeed = 600;

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Archer_Arrow::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AEnemy_Archer_Arrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_Archer_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_Archer_Arrow::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
	//UEFSM* enemy = Cast<UEFSM>(otherActor);
	if (target != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OverLap 1"));


	}
}

