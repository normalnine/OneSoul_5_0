// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Soul.generated.h"

class UNiagaraSystem;

UCLASS()
class ONESOUL_API ASoul : public AItem
{
	GENERATED_BODY()

protected:

	virtual void OnSphereOverlap(
		         UPrimitiveComponent* OverlappedComponent,
		         AActor* OthrActor,
		         UPrimitiveComponent* OtherComp,
		         int32 OtherBodyIndex,
		         bool bFromSweep,
	             const FHitResult& SweepResult) override;


private:

    UPROPERTY(EditAnywhere,Category = "Souls")
	int32 Souls = 100;


public:
     FORCEINLINE int32 GetSouls() const {return Souls;}
	 FORCEINLINE void SetSouls(int32 Numberofsouls) {Souls = Numberofsouls;}
	
	UPROPERTY(EditAnywhere, Category = "Souls")
	bool bSoulDie = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UOneSoulGameInstance* gameInst;
};
