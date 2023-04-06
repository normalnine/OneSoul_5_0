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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 requiredSouls;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 offense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxStamina;
};

UENUM(BlueprintType)
enum class ENPCState : uint8
{
	Quest,
	Incomplete,
	Complete,
	Normal
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlayerStatus> statusData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 currLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 soul = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString nickname = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ENPCState npcState = ENPCState::Quest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector lastLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInLobby = false;
};
