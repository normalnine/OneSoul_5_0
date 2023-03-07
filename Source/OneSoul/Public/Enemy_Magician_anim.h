// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_Magician_FSM.h"
#include "Enemy_Magician_anim.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UEnemy_Magician_anim : public UAnimInstance
{
	GENERATED_BODY()
	public:
		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemyState3 animState;

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
		void PlayDamageAnim(FName sectionName);
};
