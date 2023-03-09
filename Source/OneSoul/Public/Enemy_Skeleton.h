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


	// Į ����ƽ�Ž�
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* SwordMeshComp;

	// ���� ����ƽ�Ž�
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* ShieldMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Skeleton_FSM* fsm;

	//�浹 ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class USphereComponent* collisionComp;

	//�浹 ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Collision)
		class UCapsuleComponent* SwordCollisionComp;

	UFUNCTION()
		void OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
