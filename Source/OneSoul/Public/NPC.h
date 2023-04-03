// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

// UENUM(BlueprintType)
// enum class ENPCState : uint8
// {
// 	Quest,
// 	Incomplete,
// 	Complete,
// 	Normal
// };

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

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	ENPCState currState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AOnsSoulPlayer* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UNPC_InteractiveUI> interactiveUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNPC_InteractiveUI* interactiveUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UNPC_MenuUI> npcMenuUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNPC_MenuUI* npcMenuUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOneSoulGameInstance* gameInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> dialogueText_quest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> dialogueText_complete;	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> dialogueText_inComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> dialogueText_normal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bQuestComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 dialogueIndex = 0;

	UFUNCTION(BlueprintCallable)
	void BeginOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void EndOverlapTriggerBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void OpenMenuUI();
	
	UFUNCTION(BlueprintCallable)
	void PlayerInputDisable();
	
	UFUNCTION(BlueprintCallable)
	void Talk();
	
	UFUNCTION(BlueprintCallable)
	void Dialogue(TArray<FString> dialogueText);
};
