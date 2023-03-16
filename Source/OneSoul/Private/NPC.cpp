// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include <Components/BoxComponent.h>
#include <Components/WidgetComponent.h>
#include "NPC_InteractiveUI.h"
#include "NPC_MenuUI.h"
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>

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

void ANPC::BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	
	interactiveUI = CreateWidget<UNPC_InteractiveUI>(GetWorld(), interactiveUIFactory);
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
		return;
	}

	npcMenuUI = CreateWidget<UNPC_MenuUI>(GetWorld(), npcMenuUIFactory);
	if (npcMenuUI != nullptr)
	{
		npcMenuUI->AddToViewport();
		interactiveUI->RemoveFromViewport();
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->SetIgnoreLookInput(true);
	}
	
}

void ANPC::CloseMenuUI()
{
	if (npcMenuUI != nullptr)
	{
		npcMenuUI->RemoveFromViewport();
	}
}
