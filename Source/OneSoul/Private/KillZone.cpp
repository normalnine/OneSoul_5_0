 // Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "Components/BoxComponent.h"
#include "OnsSoulPlayer.h"

// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box -> SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();

	Box -> OnComponentBeginOverlap.AddDynamic(this,&AKillZone::OnBoxOverlap);
	
}

void AKillZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OthrActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  AOnsSoulPlayer* Player = Cast<AOnsSoulPlayer>(OthrActor);
  if (Player)
  {
	  Player -> Die();
	  Player ->PlayerDieTimer();
  }
}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

