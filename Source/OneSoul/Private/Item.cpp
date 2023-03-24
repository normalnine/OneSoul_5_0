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

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(ItemMesh);

	SphereCollison = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollison -> SetupAttachment(ItemMesh);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget -> SetupAttachment(GetRootComponent());

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
	ItemEffect -> SetupAttachment(GetRootComponent());

	Amplitude = 0.25f;
	TimeConstant = 5.f;

	SloatIndex = 0;

	ItemName = FString("Default");
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollison -> OnComponentBeginOverlap.AddDynamic(this,&AItem::OnSphereOverlap);
	SphereCollison -> OnComponentEndOverlap.AddDynamic(this,&AItem::OnSphereEndOverlap);

	PickupWidget -> SetVisibility(false);

	SetItemProperties(ItemState);
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

void AItem::SetItemState(EItemState State)
{
 ItemState = State;
 SetItemProperties(State);
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

void AItem::SetItemProperties(EItemState State)
{
	switch (State)
	{
	    case EItemState::EIS_Hovering:

		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;

	    case EItemState::EIS_Equipped:

		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;

		case EItemState::EIS_Falling:

		ItemMesh -> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ItemMesh -> SetSimulatePhysics(true);
		ItemMesh -> SetEnableGravity(true);
		ItemMesh -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh -> SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic,ECollisionResponse::ECR_Block);

		break;

		case EItemState::EIS_EquipInterping:

		PickupWidget -> SetVisibility(false);

		ItemMesh->SetSimulatePhysics(false);
		ItemMesh->SetEnableGravity(false);
		ItemMesh->SetVisibility(true);
		ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;

		case EItemState::EIS_PickUp:

			ItemMesh->SetSimulatePhysics(false);
			ItemMesh->SetEnableGravity(false);
			ItemMesh->SetVisibility(false);
			ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
			ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	}
}

