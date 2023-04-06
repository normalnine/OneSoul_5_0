// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_MenuUI.h"
#include <Components/Button.h>
#include <Blueprint/UserWidget.h>
#include <GameFramework/PlayerController.h>
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <Components/TextBlock.h>
#include <Components/VerticalBox.h>
#include <Components/Image.h>
#include "NPC.h"
#include "OneSoulGameInstance.h"
#include <GameFramework/HUD.h>
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "OneSoulOverlay.h"
#include <Components/HorizontalBox.h>

void UNPC_MenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (binding)
	{
		btn_levelup->OnClicked.AddDynamic(this, &UNPC_MenuUI::LevelUp);
		btn_talk->OnClicked.AddDynamic(this, &UNPC_MenuUI::Talk);
		btn_leave->OnClicked.AddDynamic(this, &UNPC_MenuUI::Leave);
		btn_yes->OnClicked.AddDynamic(this, &UNPC_MenuUI::Yes);
		btn_no->OnClicked.AddDynamic(this, &UNPC_MenuUI::No);

		player = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));
		gameInst = Cast<UOneSoulGameInstance>(GetWorld()->GetGameInstance());
		PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

		binding = false;
	}
}

void UNPC_MenuUI::LevelUp()
{	
	SetVisibility(ESlateVisibility::Hidden);
	return;
}

void UNPC_MenuUI::Talk()
{
	player->bTalking = true;
	player->TalkStartOREnd();
	
	text_dialogue->SetVisibility(ESlateVisibility::Visible);
	vb_menuBox->SetVisibility(ESlateVisibility::Hidden);
	image_back->SetVisibility(ESlateVisibility::Hidden);
	hb_next->SetVisibility(ESlateVisibility::Visible);
	return; 
}

void UNPC_MenuUI::Leave()
{
	PlayerInputEnable();
	RemoveFromViewport();
}



void UNPC_MenuUI::PlayerInputEnable()
{
	text_dialogue->SetVisibility(ESlateVisibility::Hidden);

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetIgnoreMoveInput(false);
	UE_LOG(LogTemp,Warning,TEXT("PlayerInputEnable"));
}

void UNPC_MenuUI::Yes()
{
	No();
	gameInst->npcState = ENPCState::Incomplete;
}

void UNPC_MenuUI::No()
{
	vb_menuBox->SetVisibility(ESlateVisibility::Visible);
	vb_quest->SetVisibility(ESlateVisibility::Hidden);
	image_back->SetVisibility(ESlateVisibility::Visible);
	image_quest->SetVisibility(ESlateVisibility::Hidden);
	player->TalkStartOREnd();
}
