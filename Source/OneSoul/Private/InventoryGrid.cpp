// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGrid.h"
#include "OnsSoulPlayer.h"
#include "InventorySlot.h"	
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UInventoryGrid::PreConstruct(bool IsDesignTime)
{
 
  AOnsSoulPlayer* play = Cast<AOnsSoulPlayer>(Player);

  for (int32 Index = 0; Index <= play->InventorySlots-1; Index++)
  {
     InventorySlot = CreateWidget<UInventorySlot>(GetWorld(),TInventroySlot);

     Index = InventorySlot -> GetIndex();

	 SlotsArray.Add(InventorySlot);

	 if (UniGrid)
	 {
		 UUniformGridSlot* Slots = UniGrid -> AddChildToUniformGrid(InventorySlot);
		 if (Slots)
		 {	
			 Slots -> SetRow(Index/SlotsPerRow);
			 Slots -> SetColumn(UKismetMathLibrary::Divide_IntInt(Index,SlotsPerRow));
		 }
	 }

  }
}
