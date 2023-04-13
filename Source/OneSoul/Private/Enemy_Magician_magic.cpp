// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician_magic.h"
#include <Components/SphereComponent.h>
#include "OnsSoulPlayer.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Shield.h"

// Sets default values
AEnemy_Magician_magic::AEnemy_Magician_magic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));


	RootComponent = collisionComp;
	

	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));

	bodyMeshComp->SetupAttachment(collisionComp);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));

	movementComp->SetUpdatedComponent(collisionComp);

	movementComp->InitialSpeed = 600;

	movementComp->MaxSpeed = 600;

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Magician_magic::OnOverlapBeginMagic);
}

// Called when the game starts or when spawned
void AEnemy_Magician_magic::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle ddd;
	GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Magician_magic::Die, 4.5f, false);
}

// Called every frame
void AEnemy_Magician_magic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_Magician_magic::OnOverlapBeginMagic(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
	if (target != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("player"));
		//UE_LOG(LogTemp, Warning, TEXT("OverLap 1"));
		target->ReceiveDamage(30);
		target->DirectionalHitReact(GetActorLocation());
		target->HitReactSounds();
		
		Destroy();
	}

	AShield* shield = Cast<AShield>(otherActor);
	if (shield != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("shield"));
		//플레이어 캐스팅해서
		auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());
		player = Cast<AOnsSoulPlayer>(actor);

		if (player != nullptr)
		{

			//플레이어를 넉백시킨다
			FVector imp = -1 * player->GetActorForwardVector() * 1000.0f;
			player->GetCharacterMovement()->AddImpulse(imp, true);

			//플레이어의 기력 감소
			//player->CurrentStamina = FMath::Clamp(target->CurrentStamina - 10.f, target->MinStamina, target->MaxStamina);	
		}

		Destroy();
	}
}
void AEnemy_Magician_magic::Die()
{
	Destroy();
}


