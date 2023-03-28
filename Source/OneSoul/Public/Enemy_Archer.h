// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_A.h"
#include "Enemy_Archer.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API AEnemy_Archer : public AEnemy_A
{
	GENERATED_BODY()
		AEnemy_Archer();
public:

	// 활 스태틱매쉬
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* BowMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Archer_FSM* fsm;

	//hp 바
	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar* EnemyHpBar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemy_HpBar> Enemy_HP_Widget;

	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar_WidgetComponent* HpWidget;
};
