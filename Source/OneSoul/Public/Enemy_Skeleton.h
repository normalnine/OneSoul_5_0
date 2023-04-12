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

	// Į ����ƽ�Ž�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* SwordMeshComp;

	// ���� ����ƽ�Ž�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ShieldMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSMComponent)
		class UEnemy_Skeleton_FSM* fsm;

	//�浹 ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		class UCapsuleComponent* collisionComp;

	//�浹 ������Ʈ
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

	//Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* player;

	//hp ��
	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar* EnemyHpBar;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemy_HpBar> Enemy_HP_Widget;

	UPROPERTY(EditAnywhere)
		class UEnemy_HpBar_WidgetComponent* HpWidget;

	//�÷��̾� ���� �� �浹 �Ǿ����� ������ ����Ʈ
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AActor> effectfactory;
	//�÷��̾ �и� �Ǿ����� ������ ����Ʈ
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AActor> effectfactory1;


		void reStamina();

	UFUNCTION()
	void OnOverlapME(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Į�̶� �浹�Ǹ� ������ �� ����Ʈ
	UPROPERTY(EditDefaultsOnly, Category = HitresultFactory)
	TSubclassOf<class AActor> HitresultFactory;

	//���� �ݸ����� �ٽ� Ű���Լ�
	void oncoll();

	//���п� �÷��̾� Į �¾����� ȿ����
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* blockSound;
		//���и°� �ȳ����� �Ϸ��� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool one = false;
};
