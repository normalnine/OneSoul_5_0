// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include <Components/BoxComponent.h>
#include <Components/WidgetComponent.h>
#include "NPC_InteractiveUI.h"
#include "NPC_MenuUI.h"
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerController.h>
#include <Components/TextBlock.h>
#include <Components/VerticalBox.h>
#include <Components/Image.h>
#include "OneSoulGameInstance.h"
#include <Components/HorizontalBox.h>
#include <Camera/CameraComponent.h>

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	boxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ANPC::BeginOverlapTriggerBox);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ANPC::EndOverlapTriggerBox);

	player = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));
	PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	interactiveUI = CreateWidget<UNPC_InteractiveUI>(GetWorld(), interactiveUIFactory);
	npcMenuUI = CreateWidget<UNPC_MenuUI>(GetWorld(), npcMenuUIFactory);
	gameInst = Cast<UOneSoulGameInstance>(GetWorld()->GetGameInstance());
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Q", IE_Pressed, this, &ANPC::CloseMenuUI);

}

void ANPC::BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (interactiveUI != nullptr)
	{
		interactiveUI->AddToViewport();
	}

}

void ANPC::EndOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (interactiveUI != nullptr)
	{
		interactiveUI->RemoveFromViewport();
	}
}

void ANPC::OpenMenuUI()
{
	if (npcMenuUI != nullptr)
	{
		if (npcMenuUI->IsVisible() == true)
		{
			UE_LOG(LogTemp, Warning, TEXT("menu is not null"));
			return;
		}
		
		if (interactiveUI != nullptr)
		{
			if (interactiveUI->IsVisible() == true)
			{
				interactiveUI->RemoveFromViewport();
			}
		}
		
		npcMenuUI->AddToViewport();
		//npcMenuUI->text_dialogue->SetText(FText::FromString(dialogueText[dialogueIndex]));
		PlayerInputDisable();
	}	
}

void ANPC::PlayerInputDisable()
{
	
// 	FInputModeUIOnly InputMode;
// 	InputMode.SetWidgetToFocus(npcMenuUI->TakeWidget());
// 	PlayerController->SetInputMode(InputMode);
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetIgnoreLookInput(true);
	PlayerController->SetIgnoreMoveInput(true);
}

void ANPC::Talk()
{	
	switch (gameInst->npcState)
	{

	case ENPCState::Quest:
		Dialogue(dialogueText_quest);
		break;
	case ENPCState::Complete:
		Dialogue(dialogueText_complete);
		break;
	case ENPCState::Incomplete:
		Dialogue(dialogueText_inComplete);
		break;
	case ENPCState::Normal:
		Dialogue(dialogueText_normal);
		break;

	default:
		break;
	}	
}

void ANPC::Dialogue(TArray<FString> dialogueText)
{
	dialogueIndex++;
	if (dialogueIndex > dialogueText.Num() -1)
	{
		player->bTalking = false;
		dialogueIndex = 0;

		npcMenuUI->text_dialogue->SetVisibility(ESlateVisibility::Hidden);
		npcMenuUI->hb_next->SetVisibility(ESlateVisibility::Hidden);
		
		npcMenuUI->text_dialogue->SetText(FText::FromString(dialogueText[dialogueIndex]));
		if (gameInst->npcState == ENPCState::Quest)
		{
			npcMenuUI->image_quest->SetVisibility(ESlateVisibility::Visible);
			npcMenuUI->vb_quest->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			npcMenuUI->vb_menuBox->SetVisibility(ESlateVisibility::Visible);
			npcMenuUI->image_back->SetVisibility(ESlateVisibility::Visible);
			player->TalkStartOREnd();
			if (gameInst->npcState == ENPCState::Complete)
			{
				gameInst->npcState = ENPCState::Normal;
			}
		}		
		return;
	}	
	npcMenuUI->text_dialogue->SetText(FText::FromString(dialogueText[dialogueIndex]));
}

