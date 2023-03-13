// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OneSoulGameMode.generated.h"

UCLASS(minimalapi)
class AOneSoulGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOneSoulGameMode();
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentCoins = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 BestCoins;

	void AddCoins(int32 point);

};



