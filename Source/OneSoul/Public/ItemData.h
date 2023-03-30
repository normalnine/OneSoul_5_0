// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
 GENERATED_BODY()

 UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
 TSubclassOf<class AWeapon> WeaponClass;
 UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
 class UTexture2D* ItemImage;
 UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
 float ItemCost;
};