// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "OnsSoulPlayer.h"
#include "Soul.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AItem::AItem() :
     Amplitude(0.25f),
	 TimeConstant(5.f)
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(ItemMesh);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere-> SetupAttachment(GetRootComponent());

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget -> SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect -> SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere -> OnComponentBeginOverlap.AddDynamic(this,&AItem::OnSphereOverlap);
	Sphere -> OnComponentEndOverlap.AddDynamic(this,&AItem::OnSphereEndOverlap);

	PickupWidget -> SetVisibility(false);
}

void AItem::SpawnPickupSystem()
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			PickupEffect,
			GetActorLocation());
	}
}

void AItem::SpawnPickupSound()
{
  UGameplayStatics::PlaySoundAtLocation(
                    this,
					PickupSound,
					GetActorLocation());
}

float AItem::TransformedSin()
{
 return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

void AItem::OnSphereOverlap(
            UPrimitiveComponent* OverlappedComponent,
			AActor* OthrActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult)
{
IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OthrActor);
 if (PickupInterface)
 {
	 PickupInterface->SetOverlappongItem(this);
	 PickupWidget -> SetVisibility(true);
 }
 
}

void AItem::OnSphereEndOverlap(
            UPrimitiveComponent* OverlappedComponent,
			AActor* OthrActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex)
{
	IPickUpInterface* PickupInterface = Cast<IPickUpInterface>(OthrActor);
	if (PickupInterface)
	{
		PickupInterface->SetOverlappongItem(nullptr);
		PickupWidget->SetVisibility(false);
	}
}
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
      AddActorWorldOffset(FVector(0.f,0.f,TransformedSin()));
	}



}

