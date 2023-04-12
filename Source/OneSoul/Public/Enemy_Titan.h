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

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Titan_FSM* fsm;


	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Collision)
		class UCapsuleComponent* LcollisionComp;

	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		class UCapsuleComponent* RcollisionComp;



	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool changeGroggy = false;


	//hp 바
	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar* EnemyHpBar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemy_HpBar> Enemy_HP_Widget;

	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar_WidgetComponent* HpWidget;

	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* player;
	//플레이어 스테미나 회복
	void reStamina();
	UFUNCTION()
		void OnOverlapME(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//플레이어가 패링 되었을때 나오는 이펙트
	UPROPERTY(EditAnywhere, Category = BulletFactory)
		TSubclassOf<class AActor> effectfactory1;

};
