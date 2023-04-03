// Fill out your copyright notice in the Description page of Project Settings.


#include "OneSoulOverlay.h"
#include "OneSoulGameInstance.h"
#include <Components/TextBlock.h>

void UOneSoulOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	gameInst = Cast<UOneSoulGameInstance>(GetWorld()->GetGameInstance());
}

void UOneSoulOverlay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	souls->SetText(FText::AsNumber(gameInst->soul));
}
