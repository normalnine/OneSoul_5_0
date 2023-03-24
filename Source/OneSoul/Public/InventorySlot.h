// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"


UCLASS()
class ONESOUL_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
 

private:

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Index, meta = (AllowPrivateAccess = "true"))
  int32 Index;

public:

  FORCEINLINE int32 GetIndex() const {return Index;}

};
