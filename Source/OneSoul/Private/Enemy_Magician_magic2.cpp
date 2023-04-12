// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician_magic2.h"
#include "OnsSoulPlayer.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
AEnemy_Magician_magic2::AEnemy_Magician_magic2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	RootComponent = collisionComp;
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Magician_magic2::OnOverlapBeginMagic);
}

// Called when the game starts or when spawned
void AEnemy_Magician_magic2::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle ddd;
	GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Magician_magic2::Die, 4.0f, false);


	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());
	player = Cast<AOnsSoulPlayer>(actor);
}

// Called every frame
void AEnemy_Magician_magic2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemy_Magician_magic2::OnOverlapBeginMagic(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
	if (target != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OverLap 1"));
		target->ReceiveDamage(5);
		target->DirectionalHitReact(GetActorLocation());
		target->HitReactSounds();
		FTimerHandle ddd;
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Magician_magic2::Dam, 1.5f, false);
	}
}

void AEnemy_Magician_magic2::Die()
{
	Destroy();
}

void AEnemy_Magician_magic2::Dam()
{
player->ReceiveDamage(5);
}

