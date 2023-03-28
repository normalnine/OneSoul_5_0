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

void UNPC_MenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (binding)
	{
		btn_levelup->OnClicked.AddDynamic(this, &UNPC_MenuUI::LevelUp);
		btn_talk->OnClicked.AddDynamic(this, &UNPC_MenuUI::Talk);
		btn_leave->OnClicked.AddDynamic(this, &UNPC_MenuUI::Leave);

		player = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));

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
	text_dialogue->SetVisibility(ESlateVisibility::Visible);
	vb_menuBox->SetVisibility(ESlateVisibility::Hidden);
	image_back->SetVisibility(ESlateVisibility::Hidden);
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

	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetIgnoreMoveInput(false);
	UE_LOG(LogTemp,Warning,TEXT("PlayerInputEnable"));
}
