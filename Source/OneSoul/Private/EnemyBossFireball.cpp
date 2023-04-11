// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossFireball.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <Kismet/GameplayStatics.h>
#include "OnsSoulPlayer.h"
#include <Particles/ParticleSystemComponent.h>
#include "EnemyBoss.h"
#include "EnemyBossFSM.h"
#include <GameFramework/DamageType.h>
#include <AIModule/Classes/AIController.h>

// Sets default values
AEnemyBossFireball::AEnemyBossFireball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(300.0f);

// 	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
// 	meshComp->SetupAttachment(RootComponent);
// 	meshComp->SetRelativeLocation(FVector(0, 0, -50));
// 	ConstructorHelpers::FObjectFinder <UStaticMesh>tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
// 	if (tempMesh.Succeeded())
// 	{
// 		meshComp->SetStaticMesh(tempMesh.Object);
// 	}

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	particleComp->SetupAttachment(RootComponent);
	//SetLifeSpan(2.0f);
}

// Called when the game starts or when spawned
void AEnemyBossFireball::BeginPlay()
{
	Super::BeginPlay();

	target = Cast<AOnsSoulPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass()));

	dir = target->GetActorLocation() - GetActorLocation();
	dir.GetSafeNormal();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBossFireball::BeginOverlapFireball);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &AEnemyBossFireball::EndOverlapFireball);

	enemy = Cast<AEnemyBoss>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBoss::StaticClass()));

}

// Called every frame
void AEnemyBossFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector p = GetActorLocation() + dir * DeltaTime;
	SetActorLocation(p);
}

void AEnemyBossFireball::BeginOverlapFireball(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResul)
{
	if (bOverlap)
	{
		// 플레이어 캐스팅 & 데미지 함수 호출
		target = Cast<AOnsSoulPlayer>(OtherActor);
		if (target != nullptr && target->Health > 0)
		{
			UGameplayStatics::ApplyDamage(
				target,
				100,
				enemy->fsm->ai,
				this,
				UDamageType::StaticClass());
			//AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass
			//ApplyDamage(target,1000);
			enemy->fsm->CameraShake();
;			bOverlap = false;
		}		
	}
}

void AEnemyBossFireball::EndOverlapFireball(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlap = true;
}

