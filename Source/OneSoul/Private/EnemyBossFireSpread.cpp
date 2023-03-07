// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossFireSpread.h"
#include <Engine/StaticMesh.h>
#include <Kismet/GameplayStatics.h>
#include "EnemyBoss.h"
#include <Components/BoxComponent.h>

// Sets default values
AEnemyBossFireSpread::AEnemyBossFireSpread()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(compBox);


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetupAttachment(RootComponent);
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
}

