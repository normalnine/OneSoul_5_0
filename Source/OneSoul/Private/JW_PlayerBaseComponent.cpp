// Fill out your copyright notice in the Description page of Project Settings.


#include "JW_PlayerBaseComponent.h"
#include "OnsSoulPlayer.h"
#include "OneSoulPlayerAnimInstance.h"

// Sets default values for this component's properties
UJW_PlayerBaseComponent::UJW_PlayerBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJW_PlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AOnsSoulPlayer>(GetOwner());

	playerAnim = Cast<UOneSoulPlayerAnimInstance>(me->GetMesh()->GetAnimInstance());
	
}


// Called every frame
void UJW_PlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UJW_PlayerBaseComponent::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{

}

void UJW_PlayerBaseComponent::CriAttack()
{
	playerAnim->Montage_Play(criattack);
}

void UJW_PlayerBaseComponent::SAR()
{
	playerAnim->Montage_Play(shieldAttackReact);
}
