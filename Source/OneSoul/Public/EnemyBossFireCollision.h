// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBossFireCollision.generated.h"

UCLASS()
class ONESOUL_API AEnemyBossFireCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBossFireCollision();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* shpereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBossFireSpread* fireSpread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector dir;

};
