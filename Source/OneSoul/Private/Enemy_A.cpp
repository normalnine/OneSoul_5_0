// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_A.h"

// Sets default values
AEnemy_A::AEnemy_A()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy_A::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy_A::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy_A::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

