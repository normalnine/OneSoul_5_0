// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BookUI_Trigger.generated.h"

UCLASS()
class ONESOUL_API ABookUI_Trigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABookUI_Trigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AOnsSoulPlayer* player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UBookUI> BookUIFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBookUI* bookUI;
	UFUNCTION(BlueprintCallable)
		void BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void EndOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool one = true;
};
