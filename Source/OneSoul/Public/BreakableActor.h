// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;

UCLASS()
class ONESOUL_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
	virtual void GetHit(const FVector& ImpactPoint) override;
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

private:	
    UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* GeometryCollection;
};
