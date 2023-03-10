// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JW_PlayerBaseComponent.h"
#include "JW_PlayerRollComponent.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UJW_PlayerRollComponent : public UJW_PlayerBaseComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;


	void Roll();
	void onColl();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
		UAnimMontage* Rolling;

};
