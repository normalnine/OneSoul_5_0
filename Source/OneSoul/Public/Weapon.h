// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class UBoxComponent;

UCLASS()
class ONESOUL_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
    AWeapon();
    void Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner,APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent,const FName& InSocketName);

protected:
   
    virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxOverlap(
	                 UPrimitiveComponent* OverlappedComponent,
		             AActor* OthrActor,
		             UPrimitiveComponent* OtherComp,
		             int32 OtherBodyIndex,
		             bool bFromSweep,
		             const FHitResult& SweepResult);

    UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);
private:
     
	 UPROPERTY(VisibleAnywhere,Category = "Weapon")
     UBoxComponent* WeaponBox;

	 UPROPERTY(VisibleAnywhere)
	 USceneComponent* BoxTraceStart;

	 UPROPERTY(VisibleAnywhere)
	 USceneComponent* BoxTraceEnd;

	 UPROPERTY(EditAnywhere, Category = "Weapon")
	 float Damage = 20.f;

	 UPROPERTY(EditAnywhere, Category = "Weapon")
	 float HeadDamage = 25.f;

	 bool ActorIsSameType(AActor* otherActor);

	 void BoxTrace(FHitResult& BoxHit);

	 void ExecuteGetHit(FHitResult& BoxHit);

	 UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	 FVector BoxTraceExtent = FVector(5.f);

	 UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	 bool bShowBoxDebug = false;

public:

	 TArray<AActor* > IgnoreActors;


   FORCEINLINE UBoxComponent* GetWeaponBox() const {return WeaponBox;}
};
