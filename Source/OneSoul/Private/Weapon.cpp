// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Item.h"
#include "OnsSoulPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "JW_PlayerBaseComponent.h"
#include "HitInterface.h"
#include "NormalEnemy_YG.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_FSM.h"
#include "Enemy_Magician.h"
#include "Enemy_Magician_FSM.h"
#include "Enemy_Archer.h"
#include "Enemy_Archer_FSM.h"
#include "Enemy_Titan.h"
#include "Enemy_Titan_FSM.h"


AWeapon::AWeapon()
{
 WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
 WeaponBox -> SetupAttachment(GetRootComponent());
 WeaponBox -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
 WeaponBox -> SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
 WeaponBox -> SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);

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

void AWeapon::BeginPlay()
{
 Super::BeginPlay();

 WeaponBox -> OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnBoxOverlap);

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

	      FHitResult BoxHit;
	      BoxTrace(BoxHit);

		  ANormalEnemy_YG* Enemy = Cast<ANormalEnemy_YG>(BoxHit.GetActor());
		  AEnemy_Skeleton* Enemy1 = Cast<AEnemy_Skeleton>(OthrActor);
		  AEnemy_Magician* Enemy2 = Cast<AEnemy_Magician>(OthrActor);
		  AEnemy_Archer* Enemy3 = Cast<AEnemy_Archer>(OthrActor);
		  AEnemy_Titan* Enemy4 = Cast<AEnemy_Titan>(OthrActor);
		  if (Enemy4 !=nullptr)
		  {
		  Enemy4->fsm->OnDamageProcess();
		  }
		  //매지션하고 충돌되었을때
		  if (Enemy2 !=nullptr)
		  {
			  UE_LOG(LogTemp, Warning, TEXT("AEnemy_Magician"));
			Enemy2->fsm->OnDamageProcess();
		  }
		  //아처랑 충돌되었을때
		
		  if (Enemy3 != nullptr)
		  {
			  UE_LOG(LogTemp, Warning, TEXT("AEnemy_Archer"));
			  Enemy3->fsm->OnDamageProcess();
		  }	

		  //스켈레톤이랑 충돌되었을때
		  if (Enemy1 !=nullptr)
		  {
			if (Enemy1->fsm->cri||Enemy1->fsm->Hitback)
			{	
				//플레이어를 캐스팅
				AOnsSoulPlayer* me = Cast<AOnsSoulPlayer>(GetOwner());
				//플레이어에 컴포넌트호출해서 거기에 있는 크리티컬어택 몽타주 실행
				me->compPlayerBase->CriAttack();
				//칼 콜리전 끄기 - 몬스터나 칼 콜리전 둘중 하나가 없어져야지 무한반복이 안실행됨
				WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//몬스터랑 위치맞추기
				Enemy1->SetActorLocation(me->GetActorLocation()+me->GetActorForwardVector()*100.0f);
				//몬스터 피격함수 호출
				Enemy1->fsm->OnDamageProcess();
				//공격안되던 버그를 수정하기위해
				me->IsAttacking=false;
				
			
			}
			else
			{
				Enemy1->fsm->OnDamageProcess();
				//Enemy1->fsm->moveBack();
			}
			
		  }
		if (BoxHit.GetActor())
		{
			//if (ActorIsSameType(BoxHit.GetActor())) return;

			UGameplayStatics::ApplyDamage(
			                  BoxHit.GetActor(),
							  Damage,
							  GetInstigator()->GetController(),
							  this,
							  UDamageType::StaticClass());

			ExecuteGetHit(BoxHit);
			CreateFields(BoxHit.ImpactPoint);

		}

		if (Enemy)
		{
		  Enemy->ShowHitNumer(Damage, BoxHit.Location,false);
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
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::ForDuration,
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