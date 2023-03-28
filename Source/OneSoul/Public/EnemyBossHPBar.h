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
	UPROPERTY()
	class UProgressBar* currHPBar;

	UPROPERTY()
	class UProgressBar* currHPBar2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _accumulatedDamage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _bossName;

	UPROPERTY()
	class AEnemyBoss* me;

public:
	UFUNCTION()
	void UpdateCurrHP(float curr, float max);

	UFUNCTION()
	void UpdateBackCurrHP(float curr, float max);

	UFUNCTION()
	void UpdateAccumulatedDamage(float damage);
};
