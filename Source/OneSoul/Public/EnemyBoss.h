// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "EnemyBoss.generated.h"

UCLASS()
class ONESOUL_API AEnemyBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY()
	class AOnsSoulPlayer* target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
		class UEnemyBossFSM* fsm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UEnemyBossAnim* anim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCapsuleComponent* capsuleCompHead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* sphereCompHand_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* sphereCompHand_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bOverlap = false;

	// 활성 / 비활성 하는 기능
	void SetActive(bool bActive);

	UFUNCTION()
	void BeginOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

	UFUNCTION()
		void BeginOverlapLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

	UFUNCTION()
		void BeginOverlapRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

		void ReceiveDamage(float Damage);

// 	UFUNCTION()
// 	void EndOverlapHead(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	UFUNCTION()
// 		void EndOverlapLeftHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
// 
// 	UFUNCTION()
// 		void EndOverlapRightHand(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//죽었을 때 호출되는 함수를 담는 변수
	//FEnemyBossDieDelegate dieDelegate;
};
