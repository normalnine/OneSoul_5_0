// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_Archer_FSM.h"
#include "Animation/AnimInstance.h"
#include "Enemy_Archer_anim.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UEnemy_Archer_anim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemyState5 animState;

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
		void PlayDamageAnim(FName sectionName);
};
