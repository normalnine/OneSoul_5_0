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
#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_FSM.h"
#include "Enemy_Magician.h"
#include "Enemy_Magician_FSM.h"
#include "Enemy_Archer.h"
#include "Enemy_Archer_FSM.h"
#include "Enemy_Titan.h"
#include "Enemy_Titan_FSM.h"
#include "EnemyBossFSM.h"
#include "JW_PlayerBaseComponent.h"




AWeapon::AWeapon()
{

PrimaryActorTick.bCanEverTick = true;

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

 ThrowWeaponTime = 0.7f;
 bFalling = false;
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{  
   SetOwner(NewOwner);
   SetInstigator(NewInstigator);
   FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
   GetItemMesh()->AttachToComponent(InParent,TransformRules,InSocketName);
 if (GetSphereCollision())
 {
     GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
 }
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
 FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
 GetItemMesh() -> AttachToComponent(InParent,TransformRules,InSocketName);
}

void AWeapon::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (GetItemState() == EItemState::EIS_Falling && bFalling)
  {
	  FRotator MeshRotation{0.f,GetItemMesh()->GetComponentRotation().Yaw,0.f};
	  GetItemMesh() -> SetWorldRotation(MeshRotation,false,nullptr,ETeleportType::TeleportPhysics);

  }

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
		//UE_LOG(LogTemp,Warning,TEXT("ovelapweapon"));
   
		//if (ActorIsSameType(OthrActor)) return;

	      BoxTrace(BoxHit_);

		  ANormalEnemy_YG* Enemy = Cast<ANormalEnemy_YG>(BoxHit_.GetActor());
		  AEnemy_Skeleton* Enemy1 = Cast<AEnemy_Skeleton>(OthrActor);
		  AEnemy_Magician* Enemy2 = Cast<AEnemy_Magician>(OthrActor);
		  AEnemy_Archer* Enemy3 = Cast<AEnemy_Archer>(OthrActor);
		  AEnemy_Titan* Enemy4 = Cast<AEnemy_Titan>(OthrActor);

		  if (Enemy4 != nullptr)
		  {
			  Enemy4->fsm->OnDamageProcess(2);
		  }
		  //매지션하고 충돌되었을때
		  if (Enemy2 != nullptr)
		  {
			  if (Enemy2->fsm->Hitback)
			  {
				  //플레이어를 캐스팅
				  AOnsSoulPlayer* me = Cast<AOnsSoulPlayer>(GetOwner());
				  //플레이어에 컴포넌트호출해서 거기에 있는 크리티컬어택 몽타주 실행
				  me->compPlayerBase->CriAttack();
				  //칼 콜리전 끄기 - 몬스터나 칼 콜리전 둘중 하나가 없어져야지 무한반복이 안실행됨
				  WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				  //몬스터랑 위치맞추기
				  Enemy2->SetActorLocation(me->GetActorLocation() + me->GetActorForwardVector() * 100.0f);

				  //몬스터 피격함수 호출
				  Enemy2->fsm->OnDamageProcess(100);
				  //공격모션안되던 버그를 수정하기위해
				  me->IsAttacking = false;


			  }
			  else
			  {
				  Enemy2->fsm->OnDamageProcess(100);
			  }
		  }
		  //아처랑 충돌되었을때

		  if (Enemy3 != nullptr)
		  {
			  //UE_LOG(LogTemp, Warning, TEXT("AEnemy_Archer"));
			  Enemy3->fsm->OnDamageProcess(100);
		  }

		  //스켈레톤이랑 충돌되었을때
		  if (Enemy1 != nullptr)
		  {
			  if (Enemy1->fsm->cri || Enemy1->fsm->Hitback)
			  {
				  //UE_LOG(LogTemp, Warning, TEXT("asdadasddasd"));
				  //플레이어를 캐스팅
				  AOnsSoulPlayer* me = Cast<AOnsSoulPlayer>(GetOwner());
				  //칼 콜리전 끄기 - 몬스터나 칼 콜리전 둘중 하나가 없어져야지 무한반복이 안실행됨
				  WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				   //몬스터랑 위치맞추기
				  Enemy1->SetActorLocation(me->GetActorLocation() + me->GetActorForwardVector() * 100.0f);

				  Enemy1->SetActorRotation(FRotator(0,-30,0));
				  //플레이어에 컴포넌트호출해서 거기에 있는 크리티컬어택 몽타주 실행
				  me->compPlayerBase->CriAttack();
				  //몬스터 피격함수 호출
				  Enemy1->fsm->OnDamageProcess(1);
				  //공격모션안되던 버그를 수정하기위해
				  me->IsAttacking = false;


			  }
			  else
			  {
				  WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			  }

		  }
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

// 		AEnemyBoss* Boss = Cast<AEnemyBoss>(BoxHit_.GetActor());
// 
// 		if(Boss)
// 		{
// 		 
// 			UGameplayStatics::ApplyDamage(
// 				              Boss,
// 				              100.f,
// 				              GetInstigator()->GetController(),
// 				              this,
// 				              UDamageType::StaticClass());
// 		  }
		AEnemyBoss* Boss = Cast<AEnemyBoss>(OthrActor);

		if (Boss != nullptr)
		{
			Boss->fsm->ReceiveDamage(100);
			FActorSpawnParameters SpawnParameters;
			GetWorld()->SpawnActor<AActor>(bloodEffect, BoxTraceStart->GetComponentTransform());
		}
		if (Boss == nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("bossisnull"));

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

void AWeapon::ThrowWeapon()
{
  FRotator MeshRotation{0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f};
  GetItemMesh() -> SetWorldRotation(MeshRotation,false,nullptr,ETeleportType::TeleportPhysics);

  const FVector MeshForward {GetItemMesh() -> GetForwardVector()};
  const FVector MeshRight {GetItemMesh() -> GetRightVector()};
  FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

  float RandomRotation {30.f};
  ImpulseDirection = MeshRight.RotateAngleAxis(RandomRotation,FVector(0.f,0.f,1.f));
  ImpulseDirection *= 20000.f;
  GetItemMesh() -> AddImpulse(ImpulseDirection);

  bFalling = true;
  GetWorldTimerManager().SetTimer(ThrowWeaponTimer,this,&AWeapon::StopFalliong,ThrowWeaponTime);

}

void AWeapon::StopFalliong()
{
  bFalling = false;
  SetItemState(EItemState::EIS_Hovering);
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
