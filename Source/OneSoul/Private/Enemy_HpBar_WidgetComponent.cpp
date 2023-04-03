// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_HpBar_WidgetComponent.h"
#include "Enemy_HpBar.h"
#include "Components/ProgressBar.h"

void UEnemy_HpBar_WidgetComponent::UpdateCurrHP(float curr, float max)
{
	//UE_LOG(LogTemp, Warning, TEXT("1"));

	if (HpWidget == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("2"));
		HpWidget = Cast<UEnemy_HpBar>(GetUserWidgetObject());
	}

	if (HpWidget && HpWidget->HpBar)
	{
		//UE_LOG(LogTemp, Warning, TEXT("3"));

		//Progress bar UI °»½Å
		HpWidget->UpdateCurrHP(curr,max);
	}

	
}
