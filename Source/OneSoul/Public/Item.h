// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpInterface.h"
#include "Item.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UWidgetComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped")

};

UCLASS()
class ONESOUL_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

	
protected:
	virtual void BeginPlay() override;

	virtual void SpawnPickupSystem();
	virtual void SpawnPickupSound();

	//ÁøÆø
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sine)
	float Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sine)
	float TimeConstant;

	UFUNCTION(BlueprintPure)
	float TransformedSin();

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


	EItemState ItemState = EItemState::EIS_Hovering;


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PickupWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereCollison;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	FString ItemName;
   
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= Sine, meta= (AllowPrivateAccess = "true"))
    float RunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent *ItemEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
    USoundBase* PickupSound;

public:
    
	FORCEINLINE USphereComponent* GetSphereCollision() const {return SphereCollison;}
	FORCEINLINE UWidgetComponent* GetPickupWiget() const {return PickupWidget;}
	FORCEINLINE UStaticMeshComponent* GetItemMesh() const {return ItemMesh;}
    
};
