// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JW_PlayerBaseComponent.h"
#include "JW_ParryGuardComponent.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UJW_ParryGuardComponent : public UJW_PlayerBaseComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
		UAnimMontage* Parrying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montages)
		UAnimMontage* Guard;


	void parry();
	void guard();
	void guardOff();




};
