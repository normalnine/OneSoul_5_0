// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulsRetrieved.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OnsSoulPlayer.h"
#include "OneSoul/OneSoulGameMode.h"
#include "Blueprint/UserWidget.h"

ASoulsRetrieved::ASoulsRetrieved()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh -> SetupAttachment(BoxCollision);

}

void ASoulsRetrieved::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision -> OnComponentBeginOverlap.AddDynamic(this,&ASoulsRetrieved::OnBoxOverlap);

}

void ASoulsRetrieved::OnBoxOverlap(
                      UPrimitiveComponent* OverlappedComponent,
					  AActor* OthrActor,
					  UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex,
					  bool bFromSweep,
					  const FHitResult& SweepResult)
{
 player = Cast<AOnsSoulPlayer>(OtherComp);

  if (player != nullptr)
  {
	
  }
}

void ASoulsRetrieved::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

