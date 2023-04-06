// Fill out your copyright notice in the Description page of Project Settings.


#include "BookUI_Trigger.h"
#include <Components/BoxComponent.h>
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "BookUI.h"

// Sets default values
ABookUI_Trigger::ABookUI_Trigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	boxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABookUI_Trigger::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABookUI_Trigger::BeginOverlapTriggerBox);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &ABookUI_Trigger::EndOverlapTriggerBox);
	player = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));
	bookUI = CreateWidget<UBookUI>(GetWorld(), BookUIFactory);
}

// Called every frame
void ABookUI_Trigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABookUI_Trigger::BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (one)
	{
	
		if (bookUI != nullptr)
		{
			bookUI->AddToViewport();
			one=false;
			
		}
	}
}

void ABookUI_Trigger::EndOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bookUI != nullptr)
	{
		bookUI->RemoveFromViewport();
		one=true;
	}
}

