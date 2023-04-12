// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyBossHPBar.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UEnemyBossHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProgressBar* currHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProgressBar* currHPBar2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* _accumulatedDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* _bossName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBoss* me;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateCurrHP(float curr, float max);

	UFUNCTION(BlueprintCallable)
	void UpdateBackCurrHP(float curr, float max);

	UFUNCTION(BlueprintCallable)
	void UpdateAccumulatedDamage(float damage);
};
