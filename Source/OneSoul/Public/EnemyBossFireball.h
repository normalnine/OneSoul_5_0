// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBossFireball.generated.h"

UCLASS()
class ONESOUL_API AEnemyBossFireball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBossFireball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* sphereComp;

// 	UPROPERTY(EditAnywhere)
// 		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AOnsSoulPlayer* target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBoss* enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* particleComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector dir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float fireballSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverlap = true;


	UFUNCTION(BlueprintCallable)
	void BeginOverlapFireball(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

	UFUNCTION(BlueprintCallable)
	void EndOverlapFireball(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
