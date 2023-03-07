// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossFireball.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
//#include "OneSoulCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "OnsSoulPlayer.h"
// Sets default values
AEnemyBossFireball::AEnemyBossFireball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(60.0f);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -50));
	ConstructorHelpers::FObjectFinder <UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	//SetLifeSpan(2.0f);
}

// Called when the game starts or when spawned
void AEnemyBossFireball::BeginPlay()
{
	Super::BeginPlay();

	target = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));

	dir = target->GetActorLocation() - GetActorLocation();
	dir.GetSafeNormal();
}

// Called every frame
void AEnemyBossFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector p = GetActorLocation() + dir * DeltaTime;
	SetActorLocation(p);
}

