// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneSoulGameMode.h"
#include "OneSoulCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "OneSoulOverlay.h"
#include "OnsSoulPlayer.h"
#include "Kismet/GameplayStatics.h"

AOneSoulGameMode::AOneSoulGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AOneSoulGameMode::BeginPlay()
{
 Super:: BeginPlay();


}

void AOneSoulGameMode::AddCoins(int32 point)
{
	CurrentCoins += point;
	if (CurrentCoins > BestCoins)
	{
		BestCoins = CurrentCoins;
	}
}

void AOneSoulGameMode::ReSpawnPlayer(ACharacter* player)
{
 
  UClass* Class = UGameplayStatics::GetObjectClass(player);

  GetWorld() -> GetFirstPlayerController() -> Possess(
                                              GetWorld()->SpawnActor<AOnsSoulPlayer>(Class, SpawnTransform));
  
}
