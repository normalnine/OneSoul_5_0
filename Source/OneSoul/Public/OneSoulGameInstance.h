// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Engine/DataTable.h>
#include "OneSoulGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FPlayerStatus : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		int32 level;
	UPROPERTY(EditAnywhere)
		int32 requiredSouls;
	UPROPERTY(EditAnywhere)
		int32 offense;
	UPROPERTY(EditAnywhere)
		int32 defense;
	UPROPERTY(EditAnywhere)
		float maxHP;
	UPROPERTY(EditAnywhere)
		float maxStamina;
};

UCLASS()
class ONESOUL_API UOneSoulGameInstance : public UGameInstance
{
	GENERATED_BODY()	

public:
	UOneSoulGameInstance();

// protected:
// 	virtual void Init() override;

public:
	UPROPERTY(EditAnywhere)
	TArray<FPlayerStatus> statusData;

	UPROPERTY(EditAnywhere)
	int32 currLevel = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 soul = 1000;
};
