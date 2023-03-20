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
		  //�������ϰ� �浹�Ǿ�����
		  if (Enemy2 !=nullptr)
		  {
			  UE_LOG(LogTemp, Warning, TEXT("AEnemy_Magician"));
			Enemy2->fsm->OnDamageProcess();
		  }
		  //��ó�� �浹�Ǿ�����
		
		  if (Enemy3 != nullptr)
		  {
			  UE_LOG(LogTemp, Warning, TEXT("AEnemy_Archer"));
			  Enemy3->fsm->OnDamageProcess();
		  }	

		  //���̷����̶� �浹�Ǿ�����
		  if (Enemy1 !=nullptr)
		  {
			if (Enemy1->fsm->cri||Enemy1->fsm->Hitback)
			{	
				//�÷��̾ ĳ����
				AOnsSoulPlayer* me = Cast<AOnsSoulPlayer>(GetOwner());
				//�÷��̾ ������Ʈȣ���ؼ� �ű⿡ �ִ� ũ��Ƽ�þ��� ��Ÿ�� ����
				me->compPlayerBase->CriAttack();
				//Į �ݸ��� ���� - ���ͳ� Į �ݸ��� ���� �ϳ��� ���������� ���ѹݺ��� �Ƚ����
				WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//���Ͷ� ��ġ���߱�
				Enemy1->SetActorLocation(me->GetActorLocation()+me->GetActorForwardVector()*100.0f);
				//���� �ǰ��Լ� ȣ��
				Enemy1->fsm->OnDamageProcess();
				//���ݾȵǴ� ���׸� �����ϱ�����
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