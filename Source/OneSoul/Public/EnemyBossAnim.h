// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBossFSM.h"
#include "EnemyBossAnim.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UEnemyBossAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

public:
	UPROPERTY()
		class AEnemyBoss* enemy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		EEnemyBossState state;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay0 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay1 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay2 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay3 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay4 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay5 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay6 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay7 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		bool bAttackPlay8 = false;

	UFUNCTION()
		void AnimNotify_DieEnd();

	UFUNCTION()
		void AnimNotify_AttackStartHead();
	UFUNCTION()
		void AnimNotify_AttackStartLeftHand();
	UFUNCTION()
		void AnimNotify_AttackStartRightHand();

	UFUNCTION()
		void AnimNotify_AttackEndHead();
	UFUNCTION()
		void AnimNotify_AttackEndLeftHand();
	UFUNCTION()
		void AnimNotify_AttackEndRightHand();

	UFUNCTION()
		void AnimNotify_Fireball();

	UFUNCTION()
		void AnimNotify_FireSpread();	
};
