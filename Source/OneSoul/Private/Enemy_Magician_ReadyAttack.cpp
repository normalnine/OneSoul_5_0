// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician_ReadyAttack.h"
#include "NiagaraComponent.h"

// Sets default values
AEnemy_Magician_ReadyAttack::AEnemy_Magician_ReadyAttack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	niagara_fx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AttackReady"));
	niagara_fx->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemy_Magician_ReadyAttack::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle ddd;
	GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Magician_ReadyAttack::Die, 1.5f, false);
}

// Called every frame
void AEnemy_Magician_ReadyAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy_Magician_ReadyAttack::Die()
{
	Destroy();
}

