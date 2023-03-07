// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UWidgetComponent;
class USphereComponent;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UWidgetComponent* PickupWidget;
	
protected:
	virtual void BeginPlay() override;

	//ÁøÆø
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sine, meta = (AllowPrivateAccess = "true"))
	float Amplitude;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sine, meta = (AllowPrivateAccess = "true"))
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

private:
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category= Sine,meta= (AllowPrivateAccess = "true"))
    float RunningTime;

};
