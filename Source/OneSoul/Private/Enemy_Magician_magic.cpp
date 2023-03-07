// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician_magic.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemy_Magician_magic::AEnemy_Magician_magic()
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


}

// Called when the game starts or when spawned
void AEnemy_Magician_magic::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_Magician_magic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

