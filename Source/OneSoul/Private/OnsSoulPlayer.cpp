// Fill out your copyright notice in the Description page of Project Settings.


#include "OnsSoulPlayer.h"
#include "OneSoul/OneSoulGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "JW_PlayerRollComponent.h"
#include "JW_ParryGuardComponent.h"
#include "JW_PlayerBaseComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Item.h"
#include "Weapon.h"
#include "Soul.h"
#include "OneSoulHUD.h"
#include "OneSoulOverlay.h"
#include "NormalEnemy_YG.h"
#include "ReSpawn.h"
#include "NPC.h"
#include "Shield.h"
#include "InventoryGrid.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#include "EnemyBossDieUI.h"
#include "NPC_LevelupUI.h"

#include "NPC_MenuUI.h"
#include "Interactions.h"
#include "Interactions_DialogueUI.h"
#include "EscUI.h"
#include "OneSoulGameInstance.h"
AOnsSoulPlayer::AOnsSoulPlayer()
      
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetMesh() -> SetRelativeLocationAndRotation(
	             FVector(0.f,0.f,-90.f),
				 FRotator(0.f,-90.f,0.f));
	GetMesh() -> SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh() -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh() -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh() -> SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic,ECollisionResponse::ECR_Overlap);
	GetMesh() -> SetGenerateOverlapEvents(true);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	SpringArm -> SetupAttachment(GetRootComponent());
	SpringArm -> TargetArmLength = 300.f;
	SpringArm -> bUsePawnControlRotation = true;
	SpringArm -> SetRelativeLocation(FVector(0.f,0.f,120.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm);
	Camera -> bUsePawnControlRotation = false;
	Camera -> SetRelativeRotation(FRotator(-10.f,0.f,0.f));

	Light = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	Light -> SetupAttachment(GetRootComponent());
	Light -> SetRelativeLocationAndRotation(FVector(428.964464f, -203.396706f, 365.360429f),FRotator(-49.345297f, 155.226589f, 24.157479f));
	Light -> InnerConeAngle = 36.392002f;
	Light -> OuterConeAngle = 38.312f;
	Light -> Intensity = 30000.f;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere -> SetupAttachment(GetRootComponent());

	SprintSpeed = 800.f;
	WalkSpeed = 400.f;

	
	MinStamina = 0.f;

	IsAttacking = false;
	bLMBDown = false;
	ComboCnt = 0;
	bIsAttackButton = false;

	bCanHitReact = false;

	bIsTargeting = false;
	TargetingDistance = 900.f;
	TargetingRadius = 100.f;
	TargetRotationInterpSpeed = 9.f;

	

	

	InventorySlots = 5;

	IsPaused = false;

	SetPlayerMaxSpeed(WalkSpeed);

	compPlayerRoll = CreateDefaultSubobject<UJW_PlayerRollComponent>(TEXT("Roll"));
	compPlayerGuard = CreateDefaultSubobject<UJW_ParryGuardComponent>(TEXT("ParryGuard"));
	compPlayerBase = CreateDefaultSubobject<UJW_PlayerBaseComponent>(TEXT("ParryBase"));
}

void AOnsSoulPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	
	Tags.Add(FName("OneSoulCharacter"));

	Sphere -> OnComponentBeginOverlap.AddDynamic(this,&AOnsSoulPlayer::OnSphereOverlap);
	Sphere -> OnComponentEndOverlap.AddDynamic(this,&AOnsSoulPlayer::OnSphereEndOverlap);

	ReSpawnWiget = CreateWidget<UUserWidget>(GetWorld(), Respawn);
	YouDieWiget = CreateWidget<UUserWidget>(GetWorld(),YouDie);

	MainInventory = CreateWidget<UUserWidget>(GetWorld(),MainInventorys);
	MainInventory -> SetVisibility(ESlateVisibility::Collapsed);
	MainInventory -> AddToViewport();

	GetWorld()->SpawnActor<class AActor>(PickupWeapon,GetActorTransform());
	GetWorld()->SpawnActor<class AActor>(PickupPotion, GetActorTransform());
	GetWorld()->SpawnActor<class AActor>(PickupSheid, GetActorTransform());

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && LevelStartMontage)
	{
		AnimInstance->Montage_Play(LevelStartMontage);

	}

	AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();

	AOneSoulGameMode* CurrentGameModeBase = Cast<AOneSoulGameMode>(CurrentMode);
	
	if (CurrentGameModeBase != nullptr)
	{
	  CurrentGameModeBase -> SpawnTransform = GetActorTransform();
	}

	Inventory.Add(EquippedWeapon);
	//EquippedWeapon -> SetSlotIndex(0);

	switch (RotationMode)
	{
	case ERotationMode::OrienttoCamera:
	    bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;
	case ERotationMode::OrienttoMovement:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		break;
	}

	escUI = CreateWidget<UEscUI>(GetWorld(), escUIFactory);
	gameInst = Cast<UOneSoulGameInstance>(GetWorld()->GetGameInstance());
	MaxStamina = gameInst->statusData[gameInst->currLevel].maxStamina;
	CurrentStamina = MaxStamina;
	MaxHealth = gameInst->statusData[gameInst->currLevel].maxHP;
	Health = MaxHealth;
	SoulNum = gameInst->soul;
	SpawnDefaultWeapon();
}

void AOnsSoulPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTargeting)
	{
	  UpdateTargetingControlRotation();
	}
	else if(!bIsTargeting && Taget == nullptr)
	{
		DisableLockOn();
	}
	SoulNum = gameInst->soul;

}

void AOnsSoulPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward",this,&AOnsSoulPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Move Right",this,&AOnsSoulPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right",this,&AOnsSoulPlayer::Turn);
	PlayerInputComponent->BindAxis("Look Up",this,&AOnsSoulPlayer::LookUp);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AOnsSoulPlayer::Jumpp);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AOnsSoulPlayer::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AOnsSoulPlayer::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AOnsSoulPlayer::StopSprint);
	PlayerInputComponent->BindAction("ToggleLockOn", IE_Pressed, this, &AOnsSoulPlayer::ToggleLockOnInput);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AOnsSoulPlayer::LMBDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AOnsSoulPlayer::LMBUP);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AOnsSoulPlayer::EKeyPressed);
	PlayerInputComponent->BindAction("Cancel", IE_Pressed, this, &AOnsSoulPlayer::QkeyPressed);
	PlayerInputComponent->BindAction("Esc", IE_Pressed, this, &AOnsSoulPlayer::EsckeyPressed);
	PlayerInputComponent->BindAction("Healing", IE_Pressed, this, &AOnsSoulPlayer::PotionDrinking);
	PlayerInputComponent->BindAction("WeaponChange", IE_Pressed, this, &AOnsSoulPlayer::WeaponChange);
	PlayerInputComponent->BindAction("ToggleInventory",IE_Pressed,this,&AOnsSoulPlayer::ToggleInventory);

	PlayerInputComponent->BindAction("MoveW", IE_Released, this, &AOnsSoulPlayer::notMoveF);
	PlayerInputComponent->BindAction("MoveA", IE_Released, this, &AOnsSoulPlayer::notMoveR);
	PlayerInputComponent->BindAction("MoveS", IE_Released, this, &AOnsSoulPlayer::notMoveF);
	PlayerInputComponent->BindAction("MoveD", IE_Released, this, &AOnsSoulPlayer::notMoveR);

	compPlayerRoll->SetupInputBinding(PlayerInputComponent);
	compPlayerGuard->SetupInputBinding(PlayerInputComponent);
}

void AOnsSoulPlayer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	FLatentActionInfo Stop_Move;

	if (PrevMovementMode == MOVE_Falling)
	{
		UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 1.f, Stop_Move);

		if (GetCharacterMovement()->IsFalling())
		{
		 TmpDmg = 5.f;

		 UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.25f, Stop_Move);
		 
		 if (GetCharacterMovement()->IsFalling())
		 {
		  TmpDmg = 20.f;

		  UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.25f, Stop_Move);

		  if (GetCharacterMovement()->IsFalling())
		  {
		   TmpDmg = 40.f;

		   UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.1f, Stop_Move);

		   if (GetCharacterMovement()->IsFalling())
		   {
			TmpDmg = 100.f;
		   }
		  }
		 }
		}
    }
}

float AOnsSoulPlayer::TakeDamage(
                      float Damage,
					  struct FDamageEvent const& DamageEvent,
					  AController* EventInstigator,
					  AActor* DamageCauser)
{
	if (Health - Damage <= 0.f)
	{
	  Health = 0.f;
	  Die();
	  PlayerDieTimer();
	}
	else
	{
	  DirectionalHitReact(GetActorLocation());
	  HitReactSounds();
	  ReceiveDamage(Damage);
	}

	return Damage;
}

void AOnsSoulPlayer::SetOverlappongItem(class AItem* Item)
{
  OverlappingItem = Item;
}

void AOnsSoulPlayer::AddSouls(class ASoul* Soul)
{
	if (Soul->Destroy())
	{
	  SoulNum += 1;
    }
}

void AOnsSoulPlayer::Destroyed()
{
  Super::Destroyed();

  if (UWorld* World = GetWorld())
  {
	if (AOneSoulGameMode* CurrentGameModeBase = Cast<AOneSoulGameMode>(World->GetAuthGameMode()))
	{
	  CurrentGameModeBase-> ReSpawnPlayer(this); 

      UGameplayStatics:: GetPlayerController(this,0) -> SetShowMouseCursor(false);
      UGameplayStatics:: GetPlayerController(this, 0) -> SetInputMode(FInputModeGameOnly());
	}

  }
  
}

void AOnsSoulPlayer::OnSphereOverlap(
                     UPrimitiveComponent* OverlappedComponent,
					 AActor* OthrActor,
					 UPrimitiveComponent* OtherComp,
					 int32 OtherBodyIndex,
					 bool bFromSweep,
					 const FHitResult& SweepResult)
{
  SpawnTarget = Cast<AReSpawn>(OthrActor);

}

void AOnsSoulPlayer::OnSphereEndOverlap(
                                        UPrimitiveComponent* OverlappedComponent,
										AActor* OthrActor,
										UPrimitiveComponent* OtherComp,
										int32 OtherBodyIndex)
{
 SpawnTarget = Cast<AReSpawn>(OthrActor);

 if (SpawnTarget)
 {
	 SpawnTarget = nullptr;
 }
}

void AOnsSoulPlayer::DirectionalHitReact(const FVector& ImpactPoint)
{
   
   if(IsAttacking == true) return;


	const FVector Forward = GetActorForwardVector();
	// ���� ���� ��ġ Z�� ���� �ϴ� ��� ����
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	  PlayHitReactMontage();

}

void AOnsSoulPlayer::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		EquippedWeapon -> IgnoreActors.Empty();
	}
}

void AOnsSoulPlayer::MoveForward(float Value)
{
    if(ActionState != EActionState::ECS_Unoccipied) return;
	if (Controller && (Value != 0.f))
	{
	   const FRotator ControlRotation = GetControlRotation();
	   const FRotator YawRotation(0.f,ControlRotation.Yaw,0.f);

	   const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	   AddMovementInput(Direction, Value);

	   IsMoving = true;
	   isMoveF = true;
    }
}

void AOnsSoulPlayer::MoveRight(float Value)
{
	if (ActionState != EActionState::ECS_Unoccipied) return;
	if (Controller && (Value != 0.f))
	{
	 const FRotator ControlRotation = GetControlRotation();
	 const FRotator YawRotatin(0.f, ControlRotation.Yaw, 0.f);

	 const FVector Direction = FRotationMatrix(YawRotatin).GetUnitAxis(EAxis::Y);
	 AddMovementInput(Direction, Value);

	 IsMoving = true;
	 isMoveR = true;
    }
}

void AOnsSoulPlayer::LookUp(float Value)
{
 AddControllerPitchInput(Value);
}

void AOnsSoulPlayer::Turn(float Value)
{
	if (!bIsTargeting)
	{
      AddControllerYawInput(Value);
    }
}

void AOnsSoulPlayer::StartSprint()
{
   if (CurrentStamina > MinStamina)
	{
	  UKismetSystemLibrary::K2_ClearTimer(this, "RegenerateStamina");
	  SetPlayerMaxSpeed(SprintSpeed);
	  UKismetSystemLibrary::K2_SetTimer(this, "SprintTimer", 0.1f, true);
	}
}

void AOnsSoulPlayer::StopSprint()
{
  FLatentActionInfo Stop_sp;
  UKismetSystemLibrary::K2_ClearTimer(this, "SprintTimer");
  SetPlayerMaxSpeed(WalkSpeed);
  UKismetSystemLibrary::K2_ClearTimer(this, "SprintTimer");
  UKismetSystemLibrary::RetriggerableDelay(GetWorld(), 0.2f, Stop_sp);
  if (UKismetSystemLibrary::K2_IsTimerActive(this, "SprintTimer"))
  {

  }
  else
  {
	  UKismetSystemLibrary::K2_SetTimer(this, "RegenerateStamina", 0.03f, true);
  }
}

void AOnsSoulPlayer::SpawnDefaultWeapon()
{
	if (DefaulWeaponClass)
	{
	  AWeapon* DefaultWeapon = GetWorld() -> SpawnActor<AWeapon>(DefaulWeaponClass);

	  const USkeletalMeshSocket* HandSocket = GetMesh() -> GetSocketByName(FName("RightHandSoket"));

	  if (HandSocket)
	  {
		  HandSocket -> AttachActor(DefaultWeapon,GetMesh());
	  }
    }
}

void AOnsSoulPlayer::SwapWeapon(AWeapon* WeaponToSwap)
{
  DropWeapon();
  EquipWeapon(WeaponToSwap);
  OverlappingItem = nullptr;
}

void AOnsSoulPlayer::GetPickupItem(AItem* Item)
{
 auto Weapon = Cast<AWeapon>(Item);
 if (Weapon)
 {
	 if (Inventory.Num() < INVENTORY_CAPACITY)
	 {
	   Weapon -> SetSlotIndex(Inventory.Num());
	   Inventory.Add(Weapon);
	   Weapon -> SetItemState(EItemState::EIS_PickUp);
     }
	 else
	 {
       SwapWeapon(Weapon);
	 }
 }
}

void AOnsSoulPlayer::RemoveLookOn()
{
  RetargetPlueprint->Destroy();
}

void AOnsSoulPlayer::ToggleLockOn()
{
   ANormalEnemy_YG* Enemy = Cast<ANormalEnemy_YG>(Taget);
  
    if (!bIsTargeting)
	{
		EnableLockOn();
	}
	else
	{
		DisableLockOn();
		RetargetPlueprint -> Destroy();
	}
}

void AOnsSoulPlayer::PlayHitReactMontage()
{

	 UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	 if (AnimInstance && HitReactMontage)
	 { 
		AnimInstance->Montage_Play(HitReactMontage);

        bCanHitReact = true;
	 }

}

void AOnsSoulPlayer::Attack()
{ 

		UAnimInstance* AnimInstance = (GetMesh()->GetAnimInstance());
		IsAttacking = true;

		const char* Attackist[] = { "Attack1","Attack2","Attack3" };

		if (!(AnimInstance->Montage_IsPlaying(AttackMontage)) && CanAttack())
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
		else if ((AnimInstance->Montage_IsPlaying(AttackMontage)) && CanAttack())
		{
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName(Attackist[ComboCnt]));
		}
 
}

void AOnsSoulPlayer::EKeyPressed()
{
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UEnemyBossDieUI::StaticClass(), true);

	for (UUserWidget* Widget : FoundWidgets)
	{
		Widget->RemoveFromViewport();
		return;
	}

	TArray<UUserWidget*> levelupWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), levelupWidgets, UNPC_LevelupUI::StaticClass(), true);

	for (UUserWidget* Widget : levelupWidgets)
	{		
		npc->npcMenuUI->SetVisibility(ESlateVisibility::Visible);
		npc->npcMenuUI->PlayerInputEnable();
		npc->npcMenuUI->RemoveFromViewport();
		Widget->RemoveFromViewport();
		return;
	}

	if (bTalking)
	{
		//UE_LOG(LogTemp,Warning,TEXT("TextUpdate"));
		npc->Talk();
		return;
	}



	TArray<AActor*> npcs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPC::StaticClass(), npcs);

	for (AActor* Actor : npcs)
	{
		npc = Cast<ANPC>(Actor);
		if (npc != nullptr)
		{
			if (IsOverlappingActor(npc))
			{
				npc->OpenMenuUI();
				return;
			}
		}
	}

	TArray<AActor*> interactions;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteractions::StaticClass(), interactions);

	for (AActor* Actor : interactions)
	{
		interaction = Cast<AInteractions>(Actor);
		if (interaction != nullptr)
		{
			if (IsOverlappingActor(interaction))
			{
				if (interaction->dialogueUI->IsInViewport())
				{
					interaction->Close();
					return;
				}
				else
				{
					interaction->Dialogue();
					return;
				}
			}
		}
	}
 AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
 AItem* OverlappingWidget = Cast<AItem>(OverlappingItem);
 AShield* OverlappingShiled = Cast<AShield>(OverlappingItem);
 if (OverlappingShiled)
 {
	 EquipShield(OverlappingShiled);
	 OverlappingWidget->GetPickupWiget()->SetVisibility(false);
 }
 if (OverlappingItem)
 {
	 SwapWeapon(OverlappingWeapon);
     OverlappingWidget->GetPickupWiget()->SetVisibility(false);
	
 }

 if (SpawnTarget && SpawnTarget -> GetReSpawnBox())
 {   

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && SpawnMontage)
		{
			AnimInstance->Montage_Play(SpawnMontage);

			UGameplayStatics::PlaySoundAtLocation(
				              GetWorld(),
				              SpawnSound,
				              GetActorLocation());

			PotionHP(MaxHealth);
			
			AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();

			AOneSoulGameMode* CurrentGameModeBase = Cast<AOneSoulGameMode>(CurrentMode);

			if (CurrentGameModeBase != nullptr)
			{
				CurrentGameModeBase->PotionNum = 5.f;
			}

		  PlayerSpawnTimer();
		  
         if(ReSpawnWiget == nullptr) return;
 
		  ReSpawnWiget-> AddToViewport();

	      GetWorld() -> GetTimerManager().SetTimer(SpawnWigetTimer,this,&AOnsSoulPlayer::ReSpawnRemoveWidget,2.f);

 	   }
		 
 }

 }

 void AOnsSoulPlayer::QkeyPressed()
 {
	 TArray<UUserWidget*> levlupWidgets;
	 UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), levlupWidgets, UNPC_LevelupUI::StaticClass(), true);

	 for (UUserWidget* Widget : levlupWidgets)
	 {
		 Widget->RemoveFromViewport();
		 npc->npcMenuUI->SetVisibility(ESlateVisibility::Visible);
		 return;
	 }
 }
   
 void AOnsSoulPlayer::EsckeyPressed()
 {
	if (escUI != nullptr)
	{
		if (!escUI->IsInViewport())
		{
			escUI->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

		}
		else
		{
			escUI->RemoveFromParent();
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		}
	}
 }

void AOnsSoulPlayer::SetPlayerMaxSpeed(float MaxSpeed)
{
 GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}

void AOnsSoulPlayer::SprintTimer()
{
	if (GetVelocity().Length() > 10.f)
	{

		CurrentStamina = FMath::Clamp(CurrentStamina - 1.f, MinStamina, MaxStamina);
		if (CurrentStamina <= MinStamina)
		{

			StopSprint();
		}
	}
	else
	{
		StopSprint();
	}
}

void AOnsSoulPlayer::RegenerateStamina()
{
  float OnePul = CurrentStamina + 1.f;
  CurrentStamina = FMath::Clamp(OnePul, MinStamina, MaxStamina);
  if (CurrentStamina >= MaxStamina)
   {
	 UKismetSystemLibrary::K2_ClearTimer(this, "RegenerateStamina");
   }
}

void AOnsSoulPlayer::EnableLockOn()
{
	FVector Start = this->GetActorLocation();
	FVector End = Camera->GetForwardVector() * TargetingDistance + Start;
	TEnumAsByte<EObjectTypeQuery> Pawn = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	TargetObjectTypes.Add(Pawn);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult HitResult;

	bool Result = UKismetSystemLibrary::SphereTraceSingleForObjects(
		                                GetWorld(),
		                                Start,
		                                End,
		                                TargetingRadius,
	                                 	TargetObjectTypes,
		                                false,
		                                IgnoreActors,
		                                 EDrawDebugTrace::None,
		                                 HitResult,
		                                 true);

	auto HitActor = UKismetSystemLibrary::IsValid(HitResult.GetActor());

	if (Result == true && HitActor)
	{
		TargetActor = HitResult.GetActor();
		bIsTargeting = true;
		RotationMode = ERotationMode::OrienttoCamera;
	}
}

void AOnsSoulPlayer::DisableLockOn()
{
 bIsTargeting = false;
 RotationMode = ERotationMode::OrienttoMovement;
}

void AOnsSoulPlayer::ToggleLockOnInput()
{
 ToggleLockOn();
}

void AOnsSoulPlayer::UpdateTargetingControlRotation()
{
	auto OChar = UKismetSystemLibrary::IsValid(this);
	auto Target = UKismetSystemLibrary::IsValid(TargetActor);

	if (OChar && Target)
	{

		FVector Start = this->GetActorLocation();
		FVector TargetR = TargetActor->GetActorLocation() - FVector(0.f, 0.f, 100.f);
		FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(Start, TargetR);
		FRotator CurrentRot = Controller->GetControlRotation();

		FRotator YZ = UKismetMathLibrary::RInterpTo(
			                              CurrentRot,
			                              TargetRot,
			                              UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
			                              TargetRotationInterpSpeed);


		FRotator NewRotation = UKismetMathLibrary::MakeRotator(CurrentRot.Roll, YZ.Pitch, YZ.Yaw);

		Controller->SetControlRotation(NewRotation);

		if (RetargetPlueprint == nullptr)
		{
		 RetargetPlueprint = GetWorld()->SpawnActor<class AActor>(RetargetPlueprints, TargetActor->GetActorTransform());
		}
		else
		{

			RetargetPlueprint->Destroy();

			RetargetPlueprint = GetWorld()->SpawnActor<class AActor>(RetargetPlueprints, TargetActor->GetActorTransform());
		}
	   
	}
	else
	{
		RetargetPlueprint -> Destroy();
		DisableLockOn();
	}

}

bool AOnsSoulPlayer::CanAttack()
{

	return ActionState == EActionState::ECS_Unoccipied &&
	       CharacterState != ECharacterState::ECS_Unequipped;
}

void AOnsSoulPlayer::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSoket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;

	if (EquippedWeapon == nullptr)
	{
	 EquipItemDelegate.Broadcast(-1,Weapon->GetSlotIndex());
	}
	else
	{
	  EquipItemDelegate.Broadcast(EquippedWeapon->GetSlotIndex(), Weapon -> GetSlotIndex());
	}

	EquippedWeapon = Weapon;
	EquippedWeapon -> SetItemState(EItemState::EIS_Equipped);
}

void AOnsSoulPlayer::DropWeapon()
{
	if (EquippedWeapon)
	{
	  FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld,true);
	  EquippedWeapon -> GetItemMesh()->DetachFromComponent(DetachmentTransformRules);

	  EquippedWeapon->SetItemState(EItemState::EIS_Falling);
	  EquippedWeapon->ThrowWeapon();
    }
}

void AOnsSoulPlayer::ReceiveDamage(float Damge)
{
  Health = FMath:: Clamp(Health - Damge,0.f,MaxHealth);
}

void AOnsSoulPlayer::LMBDown()
{
   
	bLMBDown = true;
	if (IsAttacking == false && CanAttack())
	{
		CurrentStamina = FMath::Clamp(CurrentStamina - 15.f, MinStamina, MaxStamina);
		if (CurrentStamina <= MinStamina)
		{ 
			UKismetSystemLibrary::K2_SetTimer(this, "RegenerateStamina", 0.1f, true);
		}
		else
		{
		  Attack();
		  AttackHitCheck();
		  UKismetSystemLibrary::K2_SetTimer(this, "RegenerateStamina", 0.1f, true);
		}
	}
	else if (IsAttacking == true && CanAttack())
	{
		bIsAttackButton = true;
	}
}

void AOnsSoulPlayer::LMBUP()
	{
		bLMBDown = false;
	}

void AOnsSoulPlayer::EndAttacking()
	{
		ActionState = EActionState::ECS_Unoccipied;
		IsAttacking = false;
	}

void AOnsSoulPlayer::AttackHitCheck()
	{
	    if (ComboCnt >= 2)
		{
		  ComboCnt = 0;
		}
		if (bIsAttackButton == true)
		{
			  ComboCnt += 1;
			  bIsAttackButton = false;
			  Attack();
		}
	}

void AOnsSoulPlayer::Die()
{

	Tags.Add(FName("Dead"));

	IsDead = true;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		UGameplayStatics::PlaySoundAtLocation(
		                   GetWorld(),
						   DeadSound,
						   GetActorLocation());
	}

	YouDieWiget->AddToViewport();

	GetWorld()->GetTimerManager().SetTimer(YouDieTimer, this, &AOnsSoulPlayer::YouDieRemoveWidget, 3.f);

	UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeUIOnly());
	if (EquippedWeapon != nullptr)
	{
	 EquippedWeapon -> Destroy();
	}
}

bool AOnsSoulPlayer::CanDisarm()
{
	return ActionState == EActionState::ECS_Unoccipied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AOnsSoulPlayer::CanArm()
{
	return ActionState == EActionState::ECS_Unoccipied&&
		   CharacterState == ECharacterState::ECS_Unequipped &&
		   EquippedWeapon;
}

void AOnsSoulPlayer::Disarm()
{
	PlayEquipMontage(FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AOnsSoulPlayer::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AOnsSoulPlayer::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AOnsSoulPlayer::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AOnsSoulPlayer::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSoket"));
	}
}

void AOnsSoulPlayer::FinishEquipping()
{
	ActionState = EActionState::ECS_Unoccipied;
}

void AOnsSoulPlayer::HitReactSounds()
{
	UGameplayStatics::PlaySoundAtLocation(
		              GetWorld(),
		              HitReactSound,
		              GetActorLocation());
}

void AOnsSoulPlayer::PotionHP(float PotionHP)
{
	if (Health > 0 && Health < 100.f)
	{
		Health += PotionHP;
		PotionHP = FMath::Clamp(Health, 0.f, MaxHealth);
    }

}

void AOnsSoulPlayer::PotionDrinking()
{

	    AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();

	    AOneSoulGameMode* CurrentGameModeBase = Cast<AOneSoulGameMode>(CurrentMode);

        if(Health >= MaxHealth) return;

	    if (CurrentGameModeBase-> PotionNum > 0)
	    {
			IsAttacking = true;
			CurrentGameModeBase-> PotionNum -= 1;
			Potion = 20.f;
			PotionHP(Potion);
			PlayPotionHealMontage();
			SetPlayerMaxSpeed(300.f);
			GetWorldTimerManager().SetTimer(PotionDrinkingTimer, this, &AOnsSoulPlayer::PotionAttakTimer, 1.5f);
		}
		else
		{
			IsAttacking = false;
		}

}

void AOnsSoulPlayer::WeaponChange()
{
	if (CanArm())
	{
	  Arm();
	  IsAttacking = false;
	  
	}
	else if (CanDisarm())
	{
		Disarm();
		IsAttacking =false;
	}
}

void AOnsSoulPlayer::ToggleInventory()
{
   
	if (IsPaused)
	{    
	    IsPaused = false;

		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
		MainInventory->SetVisibility(ESlateVisibility::Collapsed);
		UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameOnly());

		IsTab = false;
	}
	else
	{
		IsPaused = true;

		
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
		MainInventory -> SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UGameplayStatics::GetPlayerController(this, 0)->SetInputMode(FInputModeGameAndUI());

		IsTab = true;
	}

}

void AOnsSoulPlayer::PlayPotionHealMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && PotionHealMontage)
	{
		AnimInstance->Montage_Play(PotionHealMontage);
	}

	UGameplayStatics::SpawnEmitterAtLocation(
		              this,
		              PotionHealEffect,
		              GetActorLocation());
}

void AOnsSoulPlayer::PotionAttakTimer()
{
  IsAttacking = false;
}

void AOnsSoulPlayer::PlayerDie()
{
  Destroy();
}

void AOnsSoulPlayer::PlayerDieTimer()
{
 GetWorldTimerManager().SetTimer(DieTimer,this, &AOnsSoulPlayer::PlayerDie, 3.2f);
}

void AOnsSoulPlayer::PlayerSpawn()
{
UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect,SpawnTarget ->GetActorLocation());
}

void AOnsSoulPlayer::PlayerSpawnTimer()
{
 GetWorldTimerManager().SetTimer(SpawnTimer,this,&AOnsSoulPlayer::PlayerSpawn,0.5f);
}

void AOnsSoulPlayer::ReSpawnRemoveWidget()
{
	if (ReSpawnWiget != nullptr)
	{
		ReSpawnWiget->RemoveFromParent();

		ReSpawnWiget = nullptr;
	}
}

void AOnsSoulPlayer::YouDieRemoveWidget()
{
	if (YouDieWiget != nullptr)
	{
	   YouDieWiget -> RemoveFromParent();
    }

}
void AOnsSoulPlayer::notMoveF()
{
	isMoveF = false;
}
void AOnsSoulPlayer::notMoveR()
{
	isMoveR = false;
}

void AOnsSoulPlayer::ExchangeInventoryItems(int32 CurrentItemIndex, int32 NewItemIndex)
{
	if ((CurrentItemIndex == NewItemIndex) && (NewItemIndex >= Inventory.Num())) return;
	auto OldEquippedWeapon = EquippedWeapon;
	auto NewWeapon = Cast<AWeapon>(Inventory[NewItemIndex]);
	EquipWeapon(NewWeapon);

	OldEquippedWeapon -> SetItemState(EItemState::EIS_PickUp);
	NewWeapon -> SetItemState(EItemState::EIS_Equipped);
}

void AOnsSoulPlayer::EquipShield(AShield* shield)
{

	shield->Equip(GetMesh(), FName("hand_l"), this, this);
	//FVector NewScale = FVector(15.0f, 15.0f, 15.0f); // ũ�⸦ �� ��� �����մϴ�.
	//shield->SetActorRelativeScale3D(NewScale);
	canshield = true;
}

void AOnsSoulPlayer::Jumpp()
{
	if (compPlayerRoll->re)
	{
		Jump();
	}
}
