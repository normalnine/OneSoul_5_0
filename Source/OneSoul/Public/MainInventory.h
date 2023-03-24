// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainInventory.generated.h"

class UButton;
class UUserWidget;

UCLASS()
class ONESOUL_API UMainInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
virtual void Construct_Implementation();

public:

UPROPERTY(meta = (BindWidget))
UButton* EquipmentButton;

UPROPERTY(meta = (BindWidget))
UButton* InventoryButton;

UPROPERTY(meta = (BindWidget))
UButton* StateButton;

UPROPERTY(meta = (BindWidget))
UButton* MasageButton;

UPROPERTY(meta = (BindWidget))
UButton* OptionButton;

void Equipment();

void Inventory();

void State();

void Masage();

void Option();

private:

UPROPERTY(EditAnywhere)
UUserWidget* EquipmentWiget;

UPROPERTY(VisibleAnywhere)
TSubclassOf<UUserWidget> EquipmentWidgets;

UPROPERTY(EditAnywhere)
UUserWidget* InventorytWiget;

UPROPERTY(VisibleAnywhere)
TSubclassOf<UUserWidget> InventoryWidgets;

};
