// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossFireCollision.h"
#include "EnemyBossFireSpread.h"
#include <Kismet/GameplayStatics.h>
#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include <Components/SphereComponent.h>
#include <Components/BoxComponent.h>
#include "EnemyBoss.h"
#include <Components/SceneComponent.h>

// Sets default values
AEnemyBossFireCollision::AEnemyBossFireCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	shpereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Shpere Component"));
	SetRootComponent(shpereComp);

}

// Called when the game starts or when spawned
void AEnemyBossFireCollision::BeginPlay()
{
	Super::BeginPlay();
	
	fireSpread = Cast<AEnemyBossFireSpread>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBossFireSpread::StaticClass()));
	dir = fireSpread->niagara->GetComponentLocation() - fireSpread->compScene->GetComponentLocation();
	dir.Normalize();
}


// Called every frame
void AEnemyBossFireCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector p = GetActorLocation() + dir * 1000 * DeltaTime;
	SetActorLocation(p);
}

