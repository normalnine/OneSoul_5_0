// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_A.h"
#include "Enemy_Magician.generated.h"

/**
 *
 */
UCLASS()
class ONESOUL_API AEnemy_Magician : public AEnemy_A
{
	GENERATED_BODY()
public:
	AEnemy_Magician();

	// Į ����ƽ�Ž�
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaffMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Magician_FSM* fsm;

	//hp ��
	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar* EnemyHpBar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemy_HpBar> Enemy_HP_Widget;

	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar_WidgetComponent* HpWidget;
};
