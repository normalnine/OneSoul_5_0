// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBossLaser.generated.h"

UCLASS()
class ONESOUL_API AEnemyBossLaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBossLaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* rootComp;

// 	UPROPERTY(EditAnywhere)
// 	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBoss* enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* particleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* capsuleComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AOnsSoulPlayer* target;

	UFUNCTION(BlueprintCallable)
	void BeginOverlapLaser(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);
};
