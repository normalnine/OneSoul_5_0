// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy_YG.h"
#include "OneSoulGameModeBase.h"
#include "OnsSoulPlayer.h"
#include "Soul.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributeComponent.h"
#include "HealthBarComponent.h"
#include "Components/ProgressBar.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

ANormalEnemy_YG::ANormalEnemy_YG() 
{
	PrimaryActorTick.bCanEverTick = true;

	CombatRadius =500.f;
	AttackRadius =220.f;
	PatrolRadius = 200.f;

	WaitMin = 5.f;
	WaitMax = 10.f;

	ChasingSpeed = 300.f;
	PatrollingSpeed = 200.f;

	AttackMin = 0.5f;
	AttackMax = 1.f;
	BaseDamage = 20.f;

	DeathLifeSpan = 1.f;

	HitNumberDestoryTime = 1.5f;
	
	bUseControllerRotationPitch= false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement() -> bOrientRotationToMovement = true;

	GetMesh() -> SetRelativeLocationAndRotation(FVector(0.f,0.f,-90.f),FRotator(0.f,-90.f,0.f));

	GetMesh() -> SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh() -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	GetMesh() -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
	GetMesh() -> SetGenerateOverlapEvents(true);
	GetCapsuleComponent() -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxCollision -> SetupAttachment(GetMesh(), FName("hand_rSocket"));

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget -> SetupAttachment(GetRootComponent());

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing -> SightRadius = 4000.f;
	PawnSensing -> SetPeripheralVisionAngle(45.f);

}

void ANormalEnemy_YG::BeginPlay()
{
	Super::BeginPlay();

	InitializeEnemy();

	if (PawnSensing)
	{
	  PawnSensing -> OnSeePawn.AddDynamic(this,&ANormalEnemy_YG::PawnSeen);
	}

	BoxCollision -> OnComponentBeginOverlap.AddDynamic(this,&ANormalEnemy_YG::OnBoxOverlap);
	BoxCollision -> OnComponentEndOverlap.AddDynamic(this, &ANormalEnemy_YG::OnBoxEndOverlap);

	BoxCollision ->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision -> SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollision -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Overlap);

	Tags.Add(FName("Enemy"));
}

void ANormalEnemy_YG::OnBoxOverlap(
                      UPrimitiveComponent* OverlappedComponent,
					  AActor* OthrActor,
					  UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex,
					  bool bFromSweep,
					  const FHitResult& SweepResult)
{
  if(OtherComp == nullptr) return;

  auto player = Cast<AOnsSoulPlayer>(OthrActor);
  if (player)
  {
    UGameplayStatics::ApplyDamage(
	                  player,
					  BaseDamage,
					  EnemyController,
					  this,
					  UDamageType::StaticClass());
  }
}

void ANormalEnemy_YG::OnBoxEndOverlap(
                      UPrimitiveComponent* OverlappedComp,
					  AActor* OtherActor,
					  UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex)
{

}

bool ANormalEnemy_YG::InTargetRange(AActor* Target, double Radius)
{
  if(Target == nullptr) return false;
  const double DistanceToTarget = (Target -> GetActorLocation() - GetActorLocation()).Size();
  return DistanceToTarget <= Radius;
}

void ANormalEnemy_YG::MoveToTarget(AActor* Target)
{
    if(EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* ANormalEnemy_YG::ChoosePatrolTarget()
{

			TArray<AActor*> ValidTargets;
			for (auto Target : PatrolTargets)
			{
				if (Target != PartrolTarget)
				{
					ValidTargets.AddUnique(Target);
				}
			}

			const int32 NumPatrolTargets = ValidTargets.Num();
			if (NumPatrolTargets > 0)
			{
				const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
				return ValidTargets[TargetSelection];
			}

			return nullptr;
}

void ANormalEnemy_YG::CheckCombatTarget()
{
	if (CombatTarget)
	{
		if (IsOutsideCombatRadius())
		{   
		    ClearAttackTimer();
			LoseInterset();
			if (!IsEngaged())
			{
			 StartPatrolling();
			}
		}
		else if (IsOutsideAttackRadius() && !IsChasing())
		{
		  ClearAttackTimer();
		  if (!IsEngaged())
		  {
		   ChaseTarget();
		  }
		}
		else if(CanAttack())
		{
		  StartAttackTimer();
		}
	
	}
}

void ANormalEnemy_YG::CheckPatrolTarget()
{
	if (InTargetRange(PartrolTarget, PatrolRadius))
	{
		PartrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin,WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &ANormalEnemy_YG::PatrolTimerFinished, WaitTime);
	}
}

void ANormalEnemy_YG::PatrolTimerFinished()
{
  MoveToTarget(PartrolTarget);
}

void ANormalEnemy_YG::SpawnSoul()
{
	UWorld* World = GetWorld();
	if (World && SoulClass)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 0.75f);
		ASoul* SpawnSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
	}
}

void ANormalEnemy_YG::DieSound()
{
	UGameplayStatics::PlaySoundAtLocation(
		              GetWorld(),
		              DeadSound,
		              GetActorLocation());
}

bool ANormalEnemy_YG::SoulDestroy()
{
  return Soul -> bSoulDie = false;
}


void ANormalEnemy_YG::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(IsDead()) return;

	UpdateHitNumbers();

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
	 CheckCombatTarget();
	}
	else
	{
	CheckPatrolTarget();
	}

}

void ANormalEnemy_YG::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANormalEnemy_YG::GetHit(const FVector& ImpactPoint)
{
	ShowHealthBar();

	AGameModeBase* CurrentMode = GetWorld()->GetAuthGameMode();

	AOneSoulGameMode* CurrentGameModeBase = Cast<AOneSoulGameMode>(CurrentMode);

	if (IsAlive())
	{
      DirectionalHitReact(ImpactPoint);
	}
	else
	{   

		Die();
        DieSound();

		if (CurrentGameModeBase != nullptr)
		{
		  CurrentGameModeBase->AddCoins(10);
		}

		
	}

  if (HitSound)
  {
	  UGameplayStatics::PlaySoundAtLocation(
	                    this,
						HitSound,
						ImpactPoint);
  }
  if (HitParticles && GetWorld())
  {
     UGameplayStatics::SpawnEmitterAtLocation(
	                   GetWorld(),
					   HitParticles,
					   ImpactPoint);
  }
}

float ANormalEnemy_YG::TakeDamage(
                       float Damage,
					   struct FDamageEvent const& DamageEvent,
					   AController* EventInstigator,
					   AActor* DamageCauser)
{
	HandleDamage(Damage);
	CombatTarget = EventInstigator->GetPawn();

	if (IsInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius())
	{
		ChaseTarget(); 
	}

	return Damage;
}

void ANormalEnemy_YG::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PartrolTarget);
}

void ANormalEnemy_YG::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool ANormalEnemy_YG::IsOutsideCombatRadius()
{
 return !InTargetRange(CombatTarget, CombatRadius);
}

bool ANormalEnemy_YG::IsOutsideAttackRadius()
{
 return !InTargetRange(CombatTarget, AttackRadius);
}

bool ANormalEnemy_YG::IsChasing()
{
 return EnemyState == EEnemyState::EES_Chasing;
}

bool ANormalEnemy_YG::IsInsideAttackRadius()
{
 return InTargetRange(CombatTarget, AttackRadius);
}

bool ANormalEnemy_YG::IsAttacking()
{
 return EnemyState == EEnemyState::EES_Attacking;
}

bool ANormalEnemy_YG::IsDead()
{
 return EnemyState == EEnemyState::EES_Dead;
}

bool ANormalEnemy_YG::IsEngaged()
{
  return EnemyState == EEnemyState::EES_Engaged;
}

bool ANormalEnemy_YG::CanAttack()
{
 bool bCanAttack =
       IsInsideAttackRadius() && 
	   !IsAttacking() &&
	   !IsEngaged() &&
	   !IsDead();

	 return bCanAttack;

}

bool ANormalEnemy_YG::IsAlive()
{
 return Attributes && Attributes -> IsAlive();
}

void ANormalEnemy_YG::ClearPatrolTimer()
{
 GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void ANormalEnemy_YG::AttackEnd()
{
 EnemyState = EEnemyState::EES_NoState;
 CheckCombatTarget();
}

void ANormalEnemy_YG::StartAttackTimer()
{
 EnemyState = EEnemyState::EES_Attacking;
 const float AttackTime = FMath::RandRange(AttackMin,AttackMax);
 GetWorldTimerManager().SetTimer(AttackTimer,this,&ANormalEnemy_YG::Attack,AttackTime);
}

void ANormalEnemy_YG::ClearAttackTimer()
{
 GetWorldTimerManager().ClearTimer(AttackTimer);
}

void ANormalEnemy_YG::HandleDamage(float Damage)
{
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		Attributes->ReceiveDamage(Damage);
	}
}

void ANormalEnemy_YG::DisableCapsule()
{
 GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 GetMesh() -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANormalEnemy_YG::ActivateWeapon()
{
 BoxCollision -> SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ANormalEnemy_YG::DeactivateWeapon()
{
 BoxCollision -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ANormalEnemy_YG::StoreHitNumber(UUserWidget* HitNumber, FVector Location)
{
 HitNumbers.Add(HitNumber, Location);

 FTimerHandle HitNumberTimer;
 FTimerDelegate HitNumberDelegate;

 HitNumberDelegate.BindUFunction(this,FName("DestroyHitNumber"), HitNumber);
 GetWorld() -> GetTimerManager().SetTimer(HitNumberTimer,HitNumberDelegate,HitNumberDestoryTime,false);
}

void ANormalEnemy_YG::DestroyHitNumber(UUserWidget* HitNumber)
{
 HitNumbers.Remove(HitNumber);
 HitNumber -> RemoveFromParent();
}

void ANormalEnemy_YG::UpdateHitNumbers()
{
	for (auto& Hitpair : HitNumbers)
	{
	  UUserWidget* HitNumber {Hitpair.Key};
	  const FVector Location {Hitpair.Value};
	  FVector2D ScreenPosition;
	  UGameplayStatics::ProjectWorldToScreen(
	                    GetWorld()->GetFirstPlayerController(),
						Location,
						ScreenPosition);

	 HitNumber -> SetPositionInViewport(ScreenPosition);
    }
}

void ANormalEnemy_YG::HideHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void ANormalEnemy_YG::ShowHealthBar()
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void ANormalEnemy_YG::LoseInterset()
{
 CombatTarget = nullptr;
 HideHealthBar();
}

void ANormalEnemy_YG::InitializeEnemy()
{
 EnemyController = Cast<AAIController>(GetController());
 MoveToTarget(PartrolTarget);
 HideHealthBar();
}

void ANormalEnemy_YG::PawnSeen(APawn* SeenPawn)
{   
    const bool bShouldChaseTarget =
	      EnemyState != EEnemyState::EES_Dead&&
		  EnemyState != EEnemyState::EES_Chasing &&
		  EnemyState < EEnemyState::EES_Attacking &&
		  SeenPawn -> ActorHasTag(FName("OneSoulCharacter"));

	if (bShouldChaseTarget)
	{
	  CombatTarget = SeenPawn;
	  ClearPatrolTimer();
	  ChaseTarget();
	}
    
}

void ANormalEnemy_YG::DirectionalHitReact(const FVector& ImpactPoint)
{
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

	PlayHitReactMontage(Section);
}

void ANormalEnemy_YG::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ANormalEnemy_YG::Attack()
{   
  
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
	   CombatTarget = nullptr;
    }
   
   if(CombatTarget == nullptr) return;

	EnemyState = EEnemyState::EES_Engaged;
    PlayAttackMontage();
   
}

void ANormalEnemy_YG::PlayAttackMontage()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
	}

}

void ANormalEnemy_YG::Die()
{
	EnemyState = EEnemyState::EES_Dead;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		DeathPose = EDeathPose::EDP_Death;

	}
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SpawnSoul();
    SetLifeSpan(DeathLifeSpan);
	
}