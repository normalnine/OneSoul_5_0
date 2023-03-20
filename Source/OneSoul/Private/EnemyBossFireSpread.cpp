// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossFireSpread.h"
#include <Kismet/GameplayStatics.h>
#include "EnemyBoss.h"
#include "NiagaraComponent.h"
#include <Components/SceneComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
AEnemyBossFireSpread::AEnemyBossFireSpread()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(compBox);

	compScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	compScene->SetupAttachment(RootComponent);
	
// 	niagara = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
// 	niagara->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyBossFireSpread::BeginPlay()
{
	Super::BeginPlay();

	enemy = Cast<AEnemyBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBoss::StaticClass()));
}

// Called every frame
void AEnemyBossFireSpread::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorTransform(enemy->GetMesh()->GetSocketTransform(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));

	currTime += DeltaTime;
	delayTime += DeltaTime;
	if (currTime > stopTime)
	{
		return;
	}
	else
	{
		if (delayTime > 0.2f)
		{
			GetWorld()->SpawnActor<AActor>(collision, GetActorTransform());
			delayTime = 0;
		}
	}	
}
