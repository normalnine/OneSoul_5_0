// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy_A.h"
#include "Enemy_Skeleton.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API AEnemy_Skeleton : public AEnemy_A
{
	GENERATED_BODY()
public:
	AEnemy_Skeleton();

	virtual void BeginPlay() override;

	// Ä® ½ºÅÂÆ½¸Å½¬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* SwordMeshComp;

	// ¹æÆÐ ½ºÅÂÆ½¸Å½¬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ShieldMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Skeleton_FSM* fsm;

	//Ãæµ¹ ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class USphereComponent* collisionComp;

	//Ãæµ¹ ÄÄÆ÷³ÍÆ®
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* SwordCollisionComp;

	UFUNCTION()
		void OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool changeGroggy = false;

	//Å¸±ê
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* player;

};
