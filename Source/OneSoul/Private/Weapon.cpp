// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Item.h"
#include "OnsSoulPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"
#include "NormalEnemy_YG.h"
#include "EnemyBoss.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
 WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
 WeaponBox -> SetupAttachment(GetRootComponent());
 WeaponBox -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
 WeaponBox -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
 WeaponBox -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);

 BossWeaponBox = CreateDefaultSubobject<USphereComponent>(TEXT("Boss Weapon Box"));
 BossWeaponBox->SetupAttachment(GetRootComponent());
 BossWeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 BossWeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
 BossWeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

 BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
 BoxTraceStart -> SetupAttachment(GetRootComponent());

 BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
 BoxTraceEnd -> SetupAttachment(GetRootComponent());
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{  
   SetOwner(NewOwner);
   SetInstigator(NewInstigator);
   FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
   ItemMesh->AttachToComponent(InParent,TransformRules,InSocketName);
   ItemState = EItemState::EIS_Equipped;
 if (Sphere)
 {
     Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 }
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
 FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
 ItemMesh -> AttachToComponent(InParent,TransformRules,InSocketName);
}

void AWeapon::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  BoxTrace(BoxHit_);
}

void AWeapon::BeginPlay()
{
 Super::BeginPlay();

 WeaponBox -> OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnBoxOverlap);
 BossWeaponBox->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnSphereOverlap);
}

void AWeapon::OnBoxOverlap(
              UPrimitiveComponent* OverlappedComponent,
              AActor* OthrActor,
              UPrimitiveComponent* OtherComp,
              int32 OtherBodyIndex,
              bool bFromSweep,
              const FHitResult& SweepResult)
{
   
		//if (ActorIsSameType(OthrActor)) return;

	      BoxTrace(BoxHit_);

		  ANormalEnemy_YG* Enemy = Cast<ANormalEnemy_YG>(BoxHit_.GetActor());

		if (Enemy)
		{
			//if (ActorIsSameType(BoxHit.GetActor())) return;

			UGameplayStatics::ApplyDamage(
			                  Enemy,
							  Damage,
							  GetInstigator()->GetController(),
							  this,
							  UDamageType::StaticClass());

			ExecuteGetHit(BoxHit_);
		}

			CreateFields(BoxHit_.ImpactPoint);

		if (Enemy)
		{
		  Enemy->ShowHitNumer(Damage, BoxHit_.Location,false);
		}

		AEnemyBoss* Boss = Cast<AEnemyBoss>(BoxHit_.GetActor());

		if(Boss)
		{
		 
			UGameplayStatics::ApplyDamage(
				              Boss,
				              100.f,
				              GetInstigator()->GetController(),
				              this,
				              UDamageType::StaticClass());
		  }

}


void AWeapon::OnBossSphereOverlap(
              UPrimitiveComponent* OverlappedComponent,
			  AActor* OthrActor,
			  UPrimitiveComponent* OtherComp,
			  int32 OtherBodyIndex,
			  bool bFromSweep,
			  const FHitResult& SweepResult)
{
   

	AEnemyBoss* Boss = Cast<AEnemyBoss>(OthrActor);

	if (Boss != nullptr)
	{
		UGameplayStatics::ApplyDamage(
			Boss,
			100.f,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass());
	}
}

bool AWeapon::ActorIsSameType(AActor* otherActor)
{
  return GetOwner()->ActorHasTag(TEXT("Enemy")) && otherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->GetHit(BoxHit.ImpactNormal);
	}
}
