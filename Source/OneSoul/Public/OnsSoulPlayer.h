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
class AOneSoulGameMode;
class UParticleSystem;
class AReSpawn;
class USphereComponent;
class UUserWidget;
class UInventoryGrid;
class USpotLightComponent;
class AActor;

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipItemDelegate,int32, CurrentSlotIndex,int32,NewSlotIndex);

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
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void SetOverlappongItem(class AItem* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void Destroyed();

	UFUNCTION()
	virtual void OnSphereOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OthrActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(
			     UPrimitiveComponent* OverlappedComponent,
			     AActor* OthrActor,
			     UPrimitiveComponent* OtherComp,
			     int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category ="Combat")
	AWeapon* EquippedWeapon;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Combat")
	TSubclassOf<AWeapon> DefaulWeaponClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> Respawn;
	UPROPERTY(EditAnywhere)
	UUserWidget* ReSpawnWiget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> YouDie;
	UPROPERTY(EditAnywhere)
	UUserWidget* YouDieWiget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* MainInventory;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float Potion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EKey")
	bool Ekey = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool IsSpawn = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Soul")
	int32 SoulNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	bool IsMoving= false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FallDamage")
	float TmpDmg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool IsTab =false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dead")
	bool IsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "spawn")
	bool IsReSpawn;

	bool EKeyButton=false;
	UPROPERTY(EditAnywhere,Category="Retargeting")
	TSubclassOf<class AActor> RetargetPlueprints;
	UPROPERTY(EditAnywhere, Category = "Retargeting")
    class AActor* RetargetPlueprint;
	UPROPERTY(EditAnywhere, Category = "PickupActor")
	TSubclassOf<class AActor> PickupPotion;
	UPROPERTY(EditAnywhere, Category = "PickupActor")
	TSubclassOf<class AActor> PickupWeapon;
	UPROPERTY(EditAnywhere, Category = "PickupActor")
	TSubclassOf<class AActor> PickupSheid;

	FTimerHandle HitReactTimer;

	FTimerHandle PotionDrinkingTimer;

	FTimerHandle DieTimer;

	FTimerHandle SpawnTimer;

	FTimerHandle SpawnWigetTimer;

	FTimerHandle YouDieTimer;

	FTimerHandle ReTargeting;

	bool bCanHitReact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool IsPaused;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 WeaponEquipmentSlots =  3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 SheidEquipmentSlots = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 UserItemEquipmentSlots = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 LeftArrowEquipmentSlots = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 RightArrowEquipmentSlots = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 AccessorisEquipmentSlots = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 InventorySlot = 25;

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
	void DropWeapon();
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
	UFUNCTION(BlueprintCallable, Category = "Heal")
	void PotionHP(float PotionHP);
	UFUNCTION(BlueprintCallable, Category = "Heal")
	void PlayPotionHealMontage();
	UFUNCTION(BlueprintCallable, Category = "Heal")
	void PotionAttakTimer();
	UFUNCTION(BlueprintCallable, Category = "Dead")
	void PlayerDie();
	UFUNCTION(BlueprintCallable, Category = "Dead")
	void PlayerDieTimer();
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlayerSpawn();
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlayerSpawnTimer();
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ReSpawnRemoveWidget();
	UFUNCTION(BlueprintCallable, Category = "Dead")
	void YouDieRemoveWidget();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprint();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SpawnDefaultWeapon();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapWeapon(AWeapon* WeaponToSwap);
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void GetPickupItem(AItem* Item);
	UFUNCTION(BlueprintCallable, Category = "Retargeting")
	void RemoveLookOn();

protected:
	virtual void BeginPlay() override;

    void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void StartSprint();
	void ToggleLockOn();
	void LMBDown();
	void LMBUP();
	void EKeyPressed();
	void QkeyPressed();
	void EsckeyPressed();
	void PotionDrinking();
	void WeaponChange();
	void ToggleInventory();

private:
    
    ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::ECS_Unoccipied;

    UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	USpotLightComponent* Light;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainInventorys;

	UPROPERTY(VisibleAnywhere)
	class AActor* TargetActor;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* PotionHealMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* SpawnMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* DeadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* HitReactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* SpawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* PotionHealEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effect, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* SpawnEffect;

	UPROPERTY()
	UOneSoulOverlay* OneSoulOverlay;

	UPROPERTY()
	ANormalEnemy_YG* Taget;

	UPROPERTY()
	AReSpawn* SpawnTarget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AReSpawn> SpawnRe;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Inventory, meta = (AllowPrivateAccess = "true"))
	TArray<AItem*>Inventory;

	const int32 INVENTORY_CAPACITY{2};

	UPROPERTY(BlueprintAssignable,Category = Delegates, meta = (AllowPrivateAccess = "true"))
	FEquipItemDelegate EquipItemDelegate;

public:

   FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;}

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   class UJW_PlayerRollComponent* compPlayerRoll;
   UPROPERTY(EditAnywhere)
   class UJW_ParryGuardComponent* compPlayerGuard;
   UPROPERTY(EditAnywhere)
   class UJW_PlayerBaseComponent* compPlayerBase;
   UPROPERTY(EditAnywhere)
   bool isMoveF = false;
   UPROPERTY(EditAnywhere)
   bool isMoveR = false;
   UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
   UCameraComponent* Camera;
   UFUNCTION(BlueprintCallable)
   void notMoveF();
   UFUNCTION(BlueprintCallable)
   void notMoveR();
   void Jumpp();
   UFUNCTION(BlueprintCallable, Category = "Equip")
	void EquipShield(AShield* shield);
   UPROPERTY(EditAnywhere)
	   bool imguard = false;
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   bool parrying = false;
   
   bool canshield = false;

   bool bTalking = false;
   class ANPC* npc;
   class AInteractions* interaction;
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UEscUI> escUIFactory;
	class UEscUI* escUI;
	class UOneSoulGameInstance* gameInst;
	

	//플레이어 카메라 흔들리게 하려는 요소들
	bool camShake = false;
	float currCamShakeTime = 0;
	float camShakeTime = 1;
	UFUNCTION(BlueprintCallable)
	void Shake();
	float randA = 1.0f;
	float randB = 1.0f;
	float randC = 1.0f;
	float randD = 1.0f;

    AController* MeController;

   void ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex);

   void TalkStartOREnd();
   void MoveCamera();
   ACameraActor* GetNPCCamera();
};
