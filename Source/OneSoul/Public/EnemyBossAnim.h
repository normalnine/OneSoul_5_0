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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyBoss* enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEnemyBossState state;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay0 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay1 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay2 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay3 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay4 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay5 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay6 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay7 = false;

	// 공격 상태 재생할지 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay8 = false;

	UFUNCTION(BlueprintCallable)
		void AnimNotify_DieEnd();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_AttackStartHead();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_AttackStartLeftHand();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_AttackStartRightHand();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_AttackEndHead();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_AttackEndLeftHand();
	UFUNCTION(BlueprintCallable)
		void AnimNotify_AttackEndRightHand();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_Fireball();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_FireSpread();	
	UFUNCTION(BlueprintCallable)
		void AnimNotify_Laser();

	UFUNCTION(BlueprintCallable)
		void AnimNotify_Ghost();

		UFUNCTION(BlueprintCallable)
		void AnimNotify_Roar();
};
