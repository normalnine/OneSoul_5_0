// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBossGhost.generated.h"

UCLASS()
class ONESOUL_API AEnemyBossGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBossGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* sphereComp;

// 	UPROPERTY(EditAnywhere)
// 	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AOnsSoulPlayer* target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* particle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* spawnSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float traceTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currTime = 0;

	UFUNCTION(BlueprintCallable)
	void BeginOverlapGhost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);
};
