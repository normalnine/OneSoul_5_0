// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OneSoulHUD.generated.h"


UCLASS()
class ONESOUL_API AOneSoulHUD : public AHUD
{
	GENERATED_BODY()

protected:

   virtual void BeginPlay() override;
	
private:
  
  UPROPERTY(EditDefaultsOnly,Category= OneSoul)
  TSubclassOf<class UOneSoulOverlay> OneSoulOverlayClass;

  UPROPERTY()
  UOneSoulOverlay* OneSoulOverlay;

public:
  
  FORCEINLINE UOneSoulOverlay* GetOneSoulOverlay() const {return OneSoulOverlay;}
 
};
