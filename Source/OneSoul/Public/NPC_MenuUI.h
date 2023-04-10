// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPC_MenuUI.generated.h"

/**
 * 
 */
UCLASS()
class ONESOUL_API UNPC_MenuUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_levelup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_talk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_leave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* text_dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* vb_menuBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* image_back;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* image_quest;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* image_dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* vb_quest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_yes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UButton* btn_no;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* hb_next;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AOnsSoulPlayer* player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOneSoulGameInstance* gameInst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PlayerController;


	UFUNCTION(BlueprintCallable)
	void LevelUp();

	UFUNCTION(BlueprintCallable)
	void Talk();

	UFUNCTION(BlueprintCallable)
	void Leave();

	UFUNCTION(BlueprintCallable)
	void PlayerInputEnable();

	UFUNCTION(BlueprintCallable)
	void Yes();	
	UFUNCTION(BlueprintCallable)
	void No();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool binding = true;
};
