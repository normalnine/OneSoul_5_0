// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_A.h"
#include "Enemy_knight.generated.h"

/**
 *
 */
UCLASS()
class ONESOUL_API AEnemy_knight : public AEnemy_A
{
	GENERATED_BODY()
public:
	AEnemy_knight();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_knight_FSM* fsm;
	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class USphereComponent* collisionComp;

	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* SwordCollisionComp;

	UFUNCTION()
		void OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
