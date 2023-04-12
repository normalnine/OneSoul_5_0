// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_HpBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>


void UEnemy_HpBar::NativeConstruct()
{
	//progress bar hp ��������
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_HpBar")));
}
void UEnemy_HpBar::UpdateCurrHP(float curr, float max)
{
	//UE_LOG(LogTemp, Warning, TEXT("4"));
	//hp �� 0 ~ 1 ���̷� ǥ��
	float hp = curr / max;

	//Progress bar UI ����
	HpBar->SetPercent(hp);
}

void UEnemy_HpBar::UpdateDamage(float damage)
{
	_howDamage->SetText(FText::AsNumber(damage));
}
