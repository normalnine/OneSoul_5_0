// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossLaser.h"
#include <Components/CapsuleComponent.h>
#include <Components/StaticMeshComponent.h>
#include "EnemyBoss.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SceneComponent.h>

// Sets default values
AEnemyBossLaser::AEnemyBossLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);
	
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	meshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AEnemyBossLaser::BeginPlay()
{
	Super::BeginPlay();
	
	enemy = Cast<AEnemyBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBoss::StaticClass()));
}

// Called every frame
void AEnemyBossLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(enemy->GetMesh()->GetSocketLocation(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));
	SetActorRotation(enemy->GetMesh()->GetSocketRotation(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));
}

