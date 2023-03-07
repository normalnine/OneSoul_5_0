// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_Skeleton_FSM.h"
#include "Enemy_Skeleton_Anim.generated.h"

/**
 *
 */
UCLASS()
class ONESOUL_API UEnemy_Skeleton_Anim : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemyState1 animState;

	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
		void PlayDamageAnim(FName sectionName);
};
