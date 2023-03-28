// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Enemy_HpBar_WidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UEnemy_HpBar_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UEnemy_HpBar* HpWidget;

	void UpdateCurrHP(float curr, float max);
};
