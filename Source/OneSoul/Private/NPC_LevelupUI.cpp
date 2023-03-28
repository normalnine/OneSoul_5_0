// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_LevelupUI.h"
#include <Components/Button.h>
#include "OneSoulGameInstance.h"
#include <Components/TextBlock.h>
#include "NPC_MenuUI.h"
#include <Blueprint/WidgetBlueprintLibrary.h>

void UNPC_LevelupUI::NativeConstruct()
{
	Super::NativeConstruct();
	_confirm->OnClicked.AddDynamic(this, &UNPC_LevelupUI::LevelUp);
	gameInst = Cast<UOneSoulGameInstance>(GetWorld()->GetGameInstance());
	StatusInit(gameInst->currLevel);
}

void UNPC_LevelupUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (gameInst->soul >= gameInst->statusData[gameInst->currLevel].requiredSouls)
	{
		_confirm->SetIsEnabled(true);
	}
	else
	{
		_confirm->SetIsEnabled(false);
	}
}

void UNPC_LevelupUI::StatusInit(int32 currLevel)
{
	int32 afterSoul = gameInst->soul - gameInst->statusData[currLevel].requiredSouls;

	_currLevel->SetText(FText::AsNumber(gameInst->statusData[currLevel].level));
	_afterLevel->SetText(FText::AsNumber(gameInst->statusData[currLevel + 1].level));
	_currSoul->SetText(FText::AsNumber(gameInst->soul));
	_afterSoul->SetText(FText::AsNumber(afterSoul));
	_requiredSoul->SetText(FText::AsNumber(gameInst->statusData[currLevel].requiredSouls));
	_currOffense->SetText(FText::AsNumber(gameInst->statusData[currLevel].offense));
	_afterOffense->SetText(FText::AsNumber(gameInst->statusData[currLevel + 1].offense));
	_currDefense->SetText(FText::AsNumber(gameInst->statusData[currLevel].defense));
	_afterDefense->SetText(FText::AsNumber(gameInst->statusData[currLevel + 1].defense));
	_currMaxHP->SetText(FText::AsNumber(gameInst->statusData[currLevel].maxHP));
	_afterMaxHP->SetText(FText::AsNumber(gameInst->statusData[currLevel + 1].maxHP));
	_currMaxStamina->SetText(FText::AsNumber(gameInst->statusData[currLevel].maxStamina));
	_afterMaxStamina->SetText(FText::AsNumber(gameInst->statusData[currLevel + 1].maxStamina));
}

void UNPC_LevelupUI::LevelUp()
{
	gameInst->soul -= gameInst->statusData[gameInst->currLevel].requiredSouls;
	gameInst->currLevel++;
	StatusInit(gameInst->currLevel);
}
