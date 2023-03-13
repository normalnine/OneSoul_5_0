// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReSpawn.generated.h"

class UBoxComponent;

UCLASS()
class ONESOUL_API AReSpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	
	virtual void Tick(float DeltaTime) override;

	AReSpawn();

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(
			     UPrimitiveComponent* OverlappedComponent,
			     AActor* OthrActor,
			     UPrimitiveComponent* OtherComp,
			     int32 OtherBodyIndex,
			     bool bFromSweep,
			     const FHitResult& SweepResult);

private:

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* BoxCollision;


};
