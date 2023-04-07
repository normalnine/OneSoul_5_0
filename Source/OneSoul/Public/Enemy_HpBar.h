// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy_HpBar.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UEnemy_HpBar : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UProgressBar* HpBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* _howDamage;

	UFUNCTION()
		void UpdateCurrHP(float curr, float max);
	UFUNCTION()
		void UpdateDamage(float damage);

};
