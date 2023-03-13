// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OneSoulGameMode.generated.h"

class UOneSoulOverlay;
class AOnsSoulPlayer;

UCLASS(minimalapi)
class AOneSoulGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AOneSoulGameMode();

	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentCoins = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 BestCoins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform SpawnTransform;

	void AddCoins(int32 point);

	void ReSpawnPlayer(ACharacter* player);

	private:

	UPROPERTY(VisibleAnywhere)
    UOneSoulOverlay* OverlayWidget;

};



