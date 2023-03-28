// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include "NPC_InteractiveUI.h"
#include "Interactions_DialogueUI.h"
#include <GameFramework/PlayerController.h>
#include <Components/TextBlock.h>

// Sets default values
AInteractions::AInteractions()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);

	particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	particle->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AInteractions::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractions::BeginOverlapTriggerBox);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AInteractions::EndOverlapTriggerBox);

	interactiveUI = CreateWidget<UNPC_InteractiveUI>(GetWorld(), interactiveUIFactory);	
	dialogueUI = CreateWidget<UInteractions_DialogueUI>(GetWorld(), dialogueUIFactory);

	PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	dialogueUI->_dialogue->SetText(FText::FromString(dialogueString));
}

// Called every frame
void AInteractions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractions::BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (interactiveUI != nullptr)
	{
		interactiveUI->AddToViewport();
	}
}

void AInteractions::EndOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (interactiveUI != nullptr)
	{
		interactiveUI->RemoveFromViewport();
	}
	if (dialogueUI->IsInViewport())
	{
		dialogueUI->RemoveFromViewport();
	}
	PlayerController->SetIgnoreLookInput(false);
	PlayerController->SetIgnoreMoveInput(false);
}

void AInteractions::Dialogue()
{
	if (dialogueUI != nullptr)
	{
		dialogueUI->AddToViewport();
		interactiveUI->RemoveFromViewport();
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);
	}
}

void AInteractions::Close()
{
	if (dialogueUI != nullptr)
	{
		dialogueUI->RemoveFromViewport();
		if (!interactiveUI->IsInViewport())
		{
			interactiveUI->AddToViewport();
		}
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);
	}
}
