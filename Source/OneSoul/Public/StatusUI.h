// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusUI.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UStatusUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:


	UPROPERTY(EditAnywhere)
	class UOneSoulGameInstance* gameInst;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _nickname;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _currLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _currSoul;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _requiredSoul;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _currOffense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _currDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _currHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(BindWidget))
	class UTextBlock* _maxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _currStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* _maxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AOnsSoulPlayer* player;
	
	UFUNCTION(BlueprintCallable)
	void StatusInit(int32 currLevel);
};
