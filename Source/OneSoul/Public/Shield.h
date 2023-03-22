// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Shield.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API AShield : public AItem
{
	GENERATED_BODY()
public:


	AShield();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;


	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	UFUNCTION()
		void OnOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OthrActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* shieldcomp;

	//Å¸±ê
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* player;
};
