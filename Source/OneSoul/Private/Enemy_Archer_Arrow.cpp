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

	movementComp->InitialSpeed = 800;

	movementComp->MaxSpeed = 800;

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
	
	//방패에 맞았을때
	AShield*shield = Cast<AShield>(otherActor);
	if (shield !=nullptr)
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("why?"));
	}

	AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);

	if (target != nullptr)
	{
		target->ReceiveDamage(1);
		target->PlayHitReactMontage();

	}
}

