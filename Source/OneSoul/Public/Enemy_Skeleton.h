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
	virtual void Tick(float DeltaTime) override;

	// 칼 스태틱매쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* SwordMeshComp;

	// 방패 스태틱매쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ShieldMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Skeleton_FSM* fsm;

	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		class UCapsuleComponent* collisionComp;

	//충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		class UCapsuleComponent* SwordCollisionComp;

	UFUNCTION()
		void OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapBeginshield(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool changeGroggy = false;

	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* player;

	//hp 바
	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar* EnemyHpBar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemy_HpBar> Enemy_HP_Widget;

	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar_WidgetComponent* HpWidget;

	//플레이어 방패 랑 충돌 되었을때 나오는 이펙트
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AActor> effectfactory;
	//플레이어가 패링 되었을때 나오는 이펙트
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AActor> effectfactory1;


		void reStamina();

	UFUNCTION()
	void OnOverlapME(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//칼이랑 충돌되면 나오는 피 이펙트
	UPROPERTY(EditDefaultsOnly, Category = HitresultFactory)
	TSubclassOf<class AActor> HitresultFactory;

	//몸의 콜리전을 다시 키는함수
	void oncoll();

	//방패에 플레이어 칼 맞았을때 효과음
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* blockSound;
		//방패맞고 안나오게 하려고 설정한 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool one = false;
};
