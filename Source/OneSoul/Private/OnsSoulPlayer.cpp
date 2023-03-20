// Fill out your copyright notice in the Description page of Project Settings.


#include "OnsSoulPlayer.h"
#include "OneSoul/OneSoulGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera -> SetupAttachment(SpringArm);
	Camera -> bUsePawnControlRotation = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	Sphere -> SetupAttachment(GetRootComponent());

	SprintSpeed = 800.f;
	WalkSpeed = 400.f;

	MaxStamina = 100.f;
	CurrentStamina = 100.f;
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

	Health = 100.f;
	MaxHealth = 100.f;

	SoulNum = 0;

	SetPlayerMaxSpeed(WalkSpeed);
}

void AOnsSoulPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	
	Tags.Add(FName("OneSoulCharacter"));

	Sphere -> OnComponentBeginOverlap.AddDynamic(this,&AOnsSoulPlayer::OnSphereOverlap);

	ReSpawnWiget = CreateWidget<UUserWidget>(GetWorld(), Respawn);
	YouDieWiget = CreateWidget<UUserWidget>(GetWorld(),YouDie);

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

}

void AOnsSoulPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTargeting)
	{
	  UpdateTargetingControlRotation();
	}
	else
	{
		DisableLockOn();
	}

}

void AOnsSoulPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move Forward",this,&AOnsSoulPlayer::MoveForward);
	PlayerInputComponent->BindAxis("Move Right",this,&AOnsSoulPlayer::MoveRight);
	PlayerInputComponent->BindAxis("Turn Right",this,&AOnsSoulPlayer::Turn);
	PlayerInputComponent->BindAxis("Look Up",this,&AOnsSoulPlayer::LookUp);

	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&AOnsSoulPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AOnsSoulPlayer::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AOnsSoulPlayer::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AOnsSoulPlayer::StopSprint);
	PlayerInputComponent->BindAction("ToggleLockOn", IE_Pressed, this, &AOnsSoulPlayer::ToggleLockOnInput);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AOnsSoulPlayer::LMBDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AOnsSoulPlayer::LMBUP);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AOnsSoulPlayer::EKeyPressed);
	PlayerInputComponent->BindAction("Healing", IE_Pressed, this, &AOnsSoulPlayer::PotionDrinking);
	PlayerInputComponent->BindAction("WeaponChange", IE_Pressed, this, &AOnsSoulPlayer::WeaponChange);
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

void AOnsSoulPlayer::DirectionalHitReact(const FVector& ImpactPoint)
{
   
   if(IsAttacking == true) return;


	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
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
	ANPC* npc = Cast<ANPC>(UGameplayStatics::GetActorOfClass(GetWorld(), ANPC::StaticClass()));
	if (IsOverlappingActor(npc))
	{
		npc->OpenMenuUI();
		return;
	}
 AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
 AItem* OverlappingWidget = Cast<AItem>(OverlappingItem);

 if (OverlappingWeapon)
 {
	 if (EquippedWeapon)
	 {
		 EquippedWeapon -> Destroy();
	 }
     OverlappingWidget->PickupWidget->SetVisibility(false);
	 EquipWeapon(OverlappingWeapon);
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

			PlayerSpawnTimer();

         if(ReSpawnWiget == nullptr) return;
 
		  ReSpawnWiget-> AddToViewport();

	      GetWorld() -> GetTimerManager().SetTimer(SpawnWigetTimer,this,&AOnsSoulPlayer::ReSpawnRemoveWidget,2.f);
		   
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
	EquippedWeapon = Weapon;
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
		Attack();
		AttackHitCheck();
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
	IsAttacking = false;
}

void AOnsSoulPlayer::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
	IsAttacking = false;
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

        if(Health >= 100) return;

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
	}
	else if (CanDisarm())
	{
		Disarm();

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
