// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickUpInterface.h"
#include "OnsSoulPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItem;
class AWeapon;
class ASoul;
class UOneSoulOverlay;
class USoundBase;
class ANormalEnemy_YG;

UENUM(BlueprintType)
enum class EActionState : uint8
{
	ECS_Unoccipied UMETA(DisplayName = "Unoccipied"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_HitReact UMETA(DisplayName = "HitReact"),
	EAS_EquippingWeapon UMETA(DisplayName= "EquippingWeapon"),
	EAS_Stunned UMETA(DisplayName = "Stunned"),

	ECS_Max UMETA(DisplayName = "DefaultMax")
	
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
 ECS_Unequipped UMETA(DisplayName = "Unequipped"),
 ECS_EquippedOneHandedWeapon UMETA(DisplayName = "EquippedOneHandedWeapon"),
 ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "EquippedTwoHandedWeapon")
};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	OrienttoCamera,
	OrienttoMovement
};

UCLASS()
class ONESOUL_API AOnsSoulPlayer : public ACharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:
	AOnsSoulPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetOverlappongItem(class AItem* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MinStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool bIsTargeting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetRotationInterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	TArray<TEnumAsByte<EObjectTypeQuery>> TargetObjectTypes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool bLMBDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	int32 ComboCnt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	bool bIsAttackButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	FTimerHandle HitReactTimer;

	bool bCanHitReact;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ERotationMode RotationMode = ERotationMode::OrienttoMovement;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SetPlayerMaxSpeed(float MaxSpeed);
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void SprintTimer();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void RegenerateStamina();
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void EnableLockOn();
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void DisableLockOn();
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void ToggleLockOnInput();
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void UpdateTargetingControlRotation();
	UFUNCTION(BlueprintCallable, Category= "Attack")
	bool CanAttack();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EndAttacking();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void AttackHitCheck();
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void Attack();
	UFUNCTION(BlueprintCallable, Category = "Health")
	void ReceiveDamage(float Damge);
	UFUNCTION(BlueprintCallable, Category = "Hit React")
	void DirectionalHitReact(const FVector& ImpactPoint);
	UFUNCTION(BlueprintCallable, Category = "Hit React")
	void PlayHitReactMontage();
	UFUNCTION(BlueprintCallable, Category = "Dead")
	void Die();
	UFUNCTION(BlueprintCallable, Category = "Equip")
	bool CanDisarm();
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void EquipWeapon(AWeapon* Weapon);
	UFUNCTION(BlueprintCallable, Category = "Equip")
	bool CanArm();
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void Disarm();
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void Arm();
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void PlayEquipMontage(const FName& SectionName);
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void AttachWeaponToBack();
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void AttachWeaponToHand();
	UFUNCTION(BlueprintCallable, Category = "Equip")
	void FinishEquipping();
	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void HitReactSounds();

protected:
	virtual void BeginPlay() override;

    void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void StartSprint();
	void StopSprint();
	void ToggleLockOn();
	void LMBDown();
	void LMBUP();
	void EKeyPressed();
private:
    
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::ECS_Unoccipied;

    UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class AActor* TargetActor;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere)
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LevelStartMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* DeadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* HitReactSound;


	UPROPERTY()
	UOneSoulOverlay* OneSoulOverlay;

	UPROPERTY()
	ANormalEnemy_YG* Taget;

public:
   FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}
};
