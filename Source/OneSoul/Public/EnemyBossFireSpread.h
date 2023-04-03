// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBossFireSpread.generated.h"

UCLASS()
class ONESOUL_API AEnemyBossFireSpread : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBossFireSpread();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* compScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyBoss* enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf <class AActor> collision;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	class UNiagaraComponent* niagara;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float stopTime = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float delayTime = 0;
		
};
