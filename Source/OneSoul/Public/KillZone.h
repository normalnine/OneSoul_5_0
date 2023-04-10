// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillZone.generated.h"

class UBoxComponent;

UCLASS()
class ONESOUL_API AKillZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(
			     UPrimitiveComponent* OverlappedComponent,
			     AActor* OthrActor,
			     UPrimitiveComponent* OtherComp,
			     int32 OtherBodyIndex,
			     bool bFromSweep,
			     const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UBoxComponent* Box;


};
