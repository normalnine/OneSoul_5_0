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

	// Ȱ ����ƽ�Ž�
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* BowMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Archer_FSM* fsm;
};
