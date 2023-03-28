// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossHPBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include "EnemyBoss.h"
#include <Kismet/GameplayStatics.h>
#include "EnemyBossFSM.h"

void UEnemyBossHPBar::NativeConstruct()
{
	Super::NativeConstruct();
	
	me = Cast<AEnemyBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBoss::StaticClass()));

	//progress bar hp 가져오자
	currHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_bossHPBar")));
	currHPBar2 = Cast<UProgressBar>(GetWidgetFromName(TEXT("_bossHPBar_1")));
	_bossName->SetText(FText::FromString(me->fsm->bossName));
}

void UEnemyBossHPBar::UpdateCurrHP(float curr, float max)
{
	//hp 를 0 ~ 1 사이로 표현
	float hp = curr / max;

	//Progress bar UI 갱신
	currHPBar->SetPercent(hp);

}

void UEnemyBossHPBar::UpdateBackCurrHP(float curr, float max)
{
	//hp 를 0 ~ 1 사이로 표현
	float hp = curr / max;

	//Progress bar UI 갱신
	currHPBar2->SetPercent(hp);
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