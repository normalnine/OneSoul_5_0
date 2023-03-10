// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossGhost.h"
#include <Components/StaticMeshComponent.h>
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AEnemyBossGhost::AEnemyBossGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AEnemyBossGhost::BeginPlay()
{
	Super::BeginPlay();
	
	target = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));
}

// Called every frame
void AEnemyBossGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currTime += DeltaTime;

	if (currTime > traceTime)
	{
		FVector p = GetActorLocation() + dir * 20;
		SetActorLocation(p);
	}
	else
	{
		FVector CurrentLocation = GetActorLocation();

		CurrentLocation.Z += 1.5;

		SetActorLocation(CurrentLocation);

		dir = target->GetActorLocation() - GetActorLocation();
		dir.Normalize();
	}	
}

