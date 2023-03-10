// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul.h"
#include "PickUpInterface.h"
#include "NiagaraFunctionLibrary.h"

void ASoul::OnSphereOverlap(
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
		PickupInterface -> AddSouls(this);
		
		SpawnPickupSystem();
		SpawnPickupSound();

		Destroy();

		bSoulDie = true;
	}

}
