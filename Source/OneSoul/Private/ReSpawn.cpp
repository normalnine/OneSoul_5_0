// Fill out your copyright notice in the Description page of Project Settings.


#include "ReSpawn.h"
#include "Components/BoxComponent.h"
#include "OnsSoulPlayer.h"
#include "OneSoul/OneSoulGameMode.h"
#include "Kismet/GameplayStatics.h"

AReSpawn::AReSpawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
    BoxCollision -> SetupAttachment(RootComponent);
}

void AReSpawn::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision -> OnComponentBeginOverlap.AddDynamic(this,&AReSpawn::OnBoxOverlap);

}


void AReSpawn::OnBoxOverlap(
               UPrimitiveComponent* OverlappedComponent,
			   AActor* OthrActor,
			   UPrimitiveComponent* OtherComp,
			   int32 OtherBodyIndex,
			   bool bFromSweep,
			   const FHitResult& SweepResult)
{

  AOnsSoulPlayer* Player = Cast<AOnsSoulPlayer>(OthrActor);

  if (Player != nullptr)
  {
	AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();

	AOneSoulGameMode* CurrentGameModeBase = Cast<AOneSoulGameMode>(CurrentMode);
	if (CurrentGameModeBase != nullptr)
	{
		CurrentGameModeBase-> SpawnTransform = Player-> GetActorTransform();
	}
  }
}

void AReSpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

