// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossHPBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UEnemyBossHPBar::NativeConstruct()
{
	//progress bar hp ��������
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_bossHPBar")));
}

void UEnemyBossHPBar::UpdateCurrHP(float curr, float max)
{
	//hp �� 0 ~ 1 ���̷� ǥ��
	float hp = curr / max;

	//Progress bar UI ����
	currHPBar->SetPercent(hp);
}

void UEnemyBossHPBar::UpdateAccumulatedDamage(float damage)
{
	if (damage == 0)
	{
		_accumulatedDamage->SetText(FText::GetEmpty());
	}
	else
	{
		_accumulatedDamage->SetText(FText::AsNumber(damage));
	}
}
