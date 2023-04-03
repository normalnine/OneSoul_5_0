// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossLaser.h"
#include <Components/CapsuleComponent.h>
#include <Components/StaticMeshComponent.h>
#include "EnemyBoss.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SceneComponent.h>
#include <Particles/ParticleSystemComponent.h>
#include "OnsSoulPlayer.h"

// Sets default values
AEnemyBossLaser::AEnemyBossLaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	capsuleComp->SetupAttachment(RootComponent);
	
// 	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
// 	meshComp->SetupAttachment(RootComponent);
// 
// 	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
// 	if (tempMesh.Succeeded())
// 	{
// 		meshComp->SetStaticMesh(tempMesh.Object);
// 	}

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	particleComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyBossLaser::BeginPlay()
{
	Super::BeginPlay();
	
	enemy = Cast<AEnemyBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBoss::StaticClass()));
	capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBossLaser::BeginOverlapLaser);
}

// Called every frame
void AEnemyBossLaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(enemy->GetMesh()->GetSocketLocation(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));
	SetActorRotation(enemy->GetMesh()->GetSocketRotation(TEXT("MOUNTAIN_DRAGON_-Ponytail1Socket")));
}

void AEnemyBossLaser::BeginOverlapLaser(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	// 플레이어 캐스팅 & 데미지 함수 호출
	target = Cast<AOnsSoulPlayer>(OtherActor);
	if (target != nullptr)
	{
		target->ReceiveDamage(10);
		target->DirectionalHitReact(GetActorLocation());

	}
}

