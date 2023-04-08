// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoulsRetrieved.generated.h"

class UBoxComponent;
class UUserWidget;
class UStaticMeshComponent;
class AOnsSoulPlayer;

UCLASS()
class ONESOUL_API ASoulsRetrieved : public AActor
{
	GENERATED_BODY()
	
public:	

	ASoulsRetrieved();

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

public:	
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	AOnsSoulPlayer* player;

};
