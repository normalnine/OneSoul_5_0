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

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;

// 	UPROPERTY(EditAnywhere)
// 	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class AOnsSoulPlayer* target;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* particle;



	FVector dir;

	float traceTime = 3.0f;

	float currTime = 0;

	UFUNCTION()
	void BeginOverlapGhost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

	UFUNCTION()
	void EndOverlapGhost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
