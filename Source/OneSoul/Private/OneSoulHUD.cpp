// Fill out your copyright notice in the Description page of Project Settings.


#include "OneSoulHUD.h"
#include "OneSoulOverlay.h"

void AOneSoulHUD::BeginPlay()
{
  Super::BeginPlay();

  UWorld* World = GetWorld();
  if (World)
  {
	APlayerController* Controller= World -> GetFirstPlayerController();
	if (Controller && OneSoulOverlayClass)
	{
     OneSoulOverlay = CreateWidget<UOneSoulOverlay>(Controller,OneSoulOverlayClass);
	 OneSoulOverlay -> AddToViewport();
	}
  }
}
