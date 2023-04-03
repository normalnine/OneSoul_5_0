// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OneSoulOverlay.generated.h"


UCLASS()
class ONESOUL_API UOneSoulOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* souls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOneSoulGameInstance* gameInst;

};
