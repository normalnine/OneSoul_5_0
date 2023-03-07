// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Item.h"
#include "OnsSoulPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"
#include "Kismet/GameplayStatics.h"

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

void AWeapon::OnSphereOverlap(
              UPrimitiveComponent* OverlappedComponent,
              AActor* OthrActor,
              UPrimitiveComponent* OtherComp,
              int32 OtherBodyIndex,
              bool bFromSweep,
              const FHitResult& SweepResult)
{
 Super::OnSphereOverlap(
        OverlappedComponent,
        OthrActor,
        OtherComp,
        OtherBodyIndex,
        bFromSweep,
        SweepResult);

        AOnsSoulPlayer* OneSoulCharacter = Cast<AOnsSoulPlayer>(OthrActor);
        
        if (OneSoulCharacter)
        {
          
        }
     
}

void AWeapon::OnSphereEndOverlap(
              UPrimitiveComponent* OverlappedComponent,
              AActor* OthrActor,
              UPrimitiveComponent* OtherComp,
              int32 OtherBodyIndex)
{
 Super::OnSphereEndOverlap(
        OverlappedComponent,
        OthrActor,
        OtherComp,
        OtherBodyIndex);

}

void AWeapon::OnBoxOverlap(
              UPrimitiveComponent* OverlappedComponent,
              AActor* OthrActor,
              UPrimitiveComponent* OtherComp,
              int32 OtherBodyIndex,
              bool bFromSweep,
              const FHitResult& SweepResult)
{
   const FVector Start = BoxTraceStart -> GetComponentLocation();
   const FVector End =  BoxTraceEnd -> GetComponentLocation();

   TArray<AActor*> ActorsToIgnore;
   ActorsToIgnore.Add(this);
   
   FHitResult BoxHit;

   UKismetSystemLibrary::BoxTraceSingle(
                         this,
                         Start,
                         End,
                         FVector(25.f,15.f,25.f),
                         BoxTraceStart->GetComponentRotation(),
                         ETraceTypeQuery::TraceTypeQuery1,
                         false,
                         ActorsToIgnore,
                         EDrawDebugTrace::ForDuration,
                         BoxHit,
                         true);
    if (BoxHit.GetActor())
    {


      IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
      if (HitInterface)
      {
       HitInterface -> GetHit(BoxHit.ImpactPoint);
      }
      IgnoreActors.AddUnique(BoxHit.GetActor());
      CreateFields(BoxHit.ImpactPoint);

      UGameplayStatics::ApplyDamage(
                        BoxHit.GetActor(),
                        Damage,
                        GetInstigator()->GetController(),
                        this,
                        UDamageType::StaticClass());
    }
}

bool AWeapon::ActorIsSameType(AActor* otherActor)
{
  return GetOwner()->ActorHasTag(TEXT("Enemy")) && otherActor->ActorHasTag(TEXT("Enemy"));
}
