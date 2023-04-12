// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusUI.h"
#include "OneSoulGameInstance.h"
#include <Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include "OnsSoulPlayer.h"

void UStatusUI::NativeConstruct()
{
	Super::NativePreConstruct();

	gameInst = Cast<UOneSoulGameInstance>(GetWorld()->GetGameInstance());
	player = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));
	StatusInit(gameInst->currLevel);
}



void UStatusUI::StatusInit(int32 currLevel)
{	
	_nickname->SetText(FText::FromString(gameInst->nickname));
	_currLevel->SetText(FText::AsNumber(gameInst->statusData[currLevel].level));
	_currSoul->SetText(FText::AsNumber(gameInst->soul));
	_requiredSoul->SetText(FText::AsNumber(gameInst->statusData[currLevel].requiredSouls));
	_currOffense->SetText(FText::AsNumber(gameInst->statusData[currLevel].offense));
	_currDefense->SetText(FText::AsNumber(gameInst->statusData[currLevel].defense));
	_maxHP->SetText(FText::AsNumber(gameInst->statusData[currLevel].maxHP));
	_maxStamina->SetText(FText::AsNumber(gameInst->statusData[currLevel].maxStamina));
	_currHP->SetText(FText::AsNumber(gameInst->statusData[currLevel].maxHP));
	_currStamina->SetText(FText::AsNumber(gameInst->statusData[currLevel].maxStamina));
	UE_LOG(LogTemp,Error,TEXT("currhp %d"), player->Health);
	
}
