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
class AAIController;

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

	UFUNCTION(BlueprintCallable)
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
	void ShowHitNumer(int32 Damage, FVector HitLocation,bool bHeadShot);

	UFUNCTION(BlueprintCallable)
	void StoreHitNumber(UUserWidget* HitNumber, FVector Location);

	UFUNCTION()
	void DestroyHitNumber(UUserWidget* HitNumber);

	void UpdateHitNumbers();

	void PatrolTimerFinished();

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

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
    UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	AActor* CombatTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	AActor* PartrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	TArray <AActor* > PatrolTargets;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere,Category= Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitParticles;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	AAIController* EnemyController;

	UPROPERTY(EditAnywhere, Category = Combet, meta = (AllowPrivateAccess = "true"))
	double CombatRadius;

	UPROPERTY(EditAnywhere, Category = Combet, meta = (AllowPrivateAccess = "true"))
	double AttackRadius;

	UPROPERTY(EditAnywhere, Category = Combet, meta = (AllowPrivateAccess = "true"))
	double PatrolRadius;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float WaitMin;

	UPROPERTY(EditAnywhere, Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float WaitMax;

	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "AI Navigation", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(VisibleAnywhere, Category = Combet, meta = (AllowPrivateAccess = "true"))
	TMap <UUserWidget*, FVector> HitNumbers;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Combet, meta = (AllowPrivateAccess = "true"))
	FString HeadBone;

	UPROPERTY(VisibleAnywhere, Category = Combet, meta = (AllowPrivateAccess = "true"))
	float HitNumberDestoryTime;

	UPROPERTY(EditDefaultsOnly, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montage, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* DeadSound;

	public:

	FORCEINLINE FString GetHeadBone() const {return HeadBone;}

};
