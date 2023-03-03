// Copyright Epic Games, Inc. All Rights Reserved.

#include "OneSoulGameMode.h"
#include "OneSoulCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOneSoulGameMode::AOneSoulGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
