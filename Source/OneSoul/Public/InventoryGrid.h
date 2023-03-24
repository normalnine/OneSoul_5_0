// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class AOnsSoulPlayer;
class UInventorySlot;
class UUniformGridPanel;

UCLASS()
class ONESOUL_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

protected:

 virtual void PreConstruct(bool IsDesignTime);

private:

 UPROPERTY(VisibleAnywhere)
 AOnsSoulPlayer* Player;

 UPROPERTY(EditAnywhere)
 UInventorySlot* InventorySlot;
 
 UPROPERTY(EditAnywhere)
 TSubclassOf<UInventorySlot> TInventroySlot;

 UPROPERTY(EditAnywhere)
 TArray<UInventorySlot*> SlotsArray;
 
 UPROPERTY()
 UUniformGridPanel* UniGrid;

public:

int32 SlotsPerRow = 4;

};
