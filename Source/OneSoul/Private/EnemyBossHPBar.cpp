// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossHPBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UEnemyBossHPBar::NativeConstruct()
{
	//progress bar hp ��������
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_bossHPBar")));
}

void UEnemyBossHPBar::UpdateCurrHP(int32 curr, int32 max)
{
	//hp �� 0 ~ 1 ���̷� ǥ��
	float hp = curr / max;

	//Progress bar UI ����
	currHPBar->SetPercent(hp);
}

void UEnemyBossHPBar::UpdateAccumulatedDamage(int32 damage)
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
