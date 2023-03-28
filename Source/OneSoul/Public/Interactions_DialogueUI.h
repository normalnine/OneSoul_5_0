// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interactions_DialogueUI.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UInteractions_DialogueUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* _dialogue;
};
