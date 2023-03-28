// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_HpBar.h"
#include <Components/ProgressBar.h>


void UEnemy_HpBar::NativeConstruct()
{
	//progress bar hp 가져오자
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_HpBar")));
}
void UEnemy_HpBar::UpdateCurrHP(float curr, float max)
{
	UE_LOG(LogTemp, Warning, TEXT("4"));
	//hp 를 0 ~ 1 사이로 표현
	float hp = curr / max;

	//Progress bar UI 갱신
	HpBar->SetPercent(hp);
}