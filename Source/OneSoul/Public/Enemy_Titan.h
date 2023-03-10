// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_A.h"
#include "Enemy_Titan.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API AEnemy_Titan : public AEnemy_A
{
	GENERATED_BODY()
public:
	AEnemy_Titan();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Titan_FSM* fsm;


	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* LcollisionComp;

	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* RcollisionComp;



	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
