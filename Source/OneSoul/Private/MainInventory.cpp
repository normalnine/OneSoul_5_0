// Fill out your copyright notice in the Description page of Project Settings.


#include "MainInventory.h"
#include "Components/Button.h"

void UMainInventory::Construct_Implementation()
{
	EquipmentButton->OnClicked.AddDynamic(this, &UMainInventory::Equipment);
	InventoryButton->OnClicked.AddDynamic(this, &UMainInventory::Inventory);
	StateButton->OnClicked.AddDynamic(this, &UMainInventory::State);
	MasageButton->OnClicked.AddDynamic(this, &UMainInventory::Masage);
	OptionButton->OnClicked.AddDynamic(this, &UMainInventory::Option);
}

void UMainInventory::Equipment()
{
  /*SetVisibility(ESlateVisibility::Collapsed);

  EquipmentWiget = CreateWidget<UUserWidget>(GetWorld(),EquipmentWidgets);
  if (EquipmentWiget)
  {
	  EquipmentWiget -> AddToViewport();
  }*/

}

void UMainInventory::Inventory()
{
  /*SetVisibility(ESlateVisibility::Collapsed);

  InventorytWiget = CreateWidget<UUserWidget>(GetWorld(),InventoryWidgets);
  if (InventorytWiget)
  {
	  InventorytWiget -> AddToViewport();
  }*/
}

void UMainInventory::State()
{
  SetVisibility(ESlateVisibility::Collapsed);
}

void UMainInventory::Masage()
{

}

void UMainInventory::Option()
{
 SetVisibility(ESlateVisibility::Collapsed);
}
