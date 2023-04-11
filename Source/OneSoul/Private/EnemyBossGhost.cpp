// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossGhost.h"
#include <Components/StaticMeshComponent.h>
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>
#include <Components/SphereComponent.h>
#include "EnemyBoss.h"
#include "EnemyBossFSM.h"
#include <GameFramework/DamageType.h>
#include <AIModule/Classes/AIController.h>

// Sets default values
AEnemyBossGhost::AEnemyBossGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);

// 	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
// 
// 	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
// 	if (tempMesh.Succeeded())
// 	{
// 		meshComp->SetStaticMesh(tempMesh.Object);
// 	}

	particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	particle->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemyBossGhost::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBossGhost::BeginOverlapGhost);
	
	target = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), spawnSound, GetActorLocation());
	enemy = Cast<AEnemyBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBoss::StaticClass()));
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

void AEnemyBossGhost::BeginOverlapGhost(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	// 플레이어 캐스팅 & 데미지 함수 호출
	target = Cast<AOnsSoulPlayer>(OtherActor);
	if (target != nullptr && target->Health > 0)
	{
		UGameplayStatics::ApplyDamage(
			target,
			100.f,
			enemy->fsm->ai,
			this,
			UDamageType::StaticClass());
		enemy->fsm->CameraShake();
		enemy->fsm->Roar(1000.0f);
	}
}
