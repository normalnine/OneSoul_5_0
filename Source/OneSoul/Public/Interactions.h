// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactions.generated.h"

UCLASS()
class ONESOUL_API AInteractions : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInteractions();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UNPC_InteractiveUI> interactiveUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNPC_InteractiveUI* interactiveUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UInteractions_DialogueUI> dialogueUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInteractions_DialogueUI* dialogueUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString dialogueString;

	UFUNCTION(BlueprintCallable)
	void BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EndOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Dialogue();
	
	UFUNCTION(BlueprintCallable)
	void Close();


};
