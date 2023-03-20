// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

UCLASS()
class ONESOUL_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class AOnsSoulPlayer* player;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UNPC_InteractiveUI> interactiveUIFactory;

	UPROPERTY(EditAnywhere)
	class UNPC_InteractiveUI* interactiveUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UNPC_MenuUI> npcMenuUIFactory;

	UPROPERTY(EditAnywhere)
	class UNPC_MenuUI* npcMenuUI;

	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> dialogueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 dialogueIndex = 0;

	UFUNCTION()
	void BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul);

	UFUNCTION()
	void EndOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OpenMenuUI();
	
	UFUNCTION()
	void PlayerInputDisable();
	
	UFUNCTION()
	void Dialogue();
};
