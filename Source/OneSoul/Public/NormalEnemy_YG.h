// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "NormalEnemy_YG.generated.h"

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_Alive UMETA(DisplayName = "Alive"),
	EDP_Death UMETA(DisplayName = "Death"),
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Dead  UMETA(DisplayName = "Dead"),
	EES_Engaged UMETA(DisplayName = "Engaged"),

	EES_NoState UMETA(DisplayName = "NoState")


};

class UAnimMontage;
class USoundBase;
class UParticleSystem;
class UAttributeComponent;
class AActor;
class AAIController;
class UHealthBarComponent;
class UPawnSensingComponent;
class UBoxComponent;

UCLASS()
class ONESOUL_API ANormalEnemy_YG : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ANormalEnemy_YG();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit(const FVector& ImpactPoint) override;

	//virtual void GetHit(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed;

	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsChasing();
	bool IsInsideAttackRadius();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	bool CanAttack();
	bool IsAlive();
	void ClearPatrolTimer();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void StartAttackTimer();
	void ClearAttackTimer();

	void HandleDamage(float Damage);
	void DisableCapsule();

	UFUNCTION(BlueprintCallable)
	void ActivateWeapon();
	UFUNCTION(BlueprintCallable)
	void DeactivateWeapon();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category= Combat)
	float AttackMin;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowHitNumer(int32 Damage, FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OthrActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual	void OnBoxEndOverlap(
	             UPrimitiveComponent* OverlappedComp,
				 AActor* OtherActor,
				 UPrimitiveComponent* OtherComp,
				 int32 OtherBodyIndex);
 
	void Die();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterset();
	void InitializeEnemy();
	void PlayHitReactMontage(const FName& SectionName);

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
    
	void DirectionalHitReact(const FVector& ImpactPoint);

	void Attack();

	void PlayAttackMontage();


private:

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
    UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere,Category= Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere)
	double CombatRadius;

	UPROPERTY(EditAnywhere)
	double AttackRadius;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PartrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray <AActor* > PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius;

	void PatrolTimerFinished();

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax;

	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	AActor* CombatTarget;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* DeadSound;

	UPROPERTY(VisibleAnywhere, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TMap <UUserWidget*, FVector> HitNumbers;

	UPROPERTY(VisibleAnywhere, Category = Combet, meta = (AllowPrivateAccess = "true"))
	float HitNumberDestoryTime;

};
