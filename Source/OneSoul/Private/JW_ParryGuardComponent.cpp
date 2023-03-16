// Fill out your copyright notice in the Description page of Project Settings.


#include "JW_ParryGuardComponent.h"
#include "OnsSoulPlayer.h"
#include "Components/CapsuleComponent.h"
#include "OneSoulPlayerAnimInstance.h"


void UJW_ParryGuardComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UJW_ParryGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UJW_ParryGuardComponent::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{

	Super::SetupInputBinding(PlayerInputComponent);
	//ctrl
	PlayerInputComponent->BindAction(TEXT("Parry"), IE_Pressed, this, &UJW_ParryGuardComponent::parry);

	//R_button
	PlayerInputComponent->BindAction(TEXT("Guard"),IE_Pressed,this,&UJW_ParryGuardComponent::guard);
	PlayerInputComponent->BindAction(TEXT("Guard"), IE_Released, this, &UJW_ParryGuardComponent::guardOff);
}

void UJW_ParryGuardComponent::parry()
{
	if (!(playerAnim->Montage_IsPlaying(Parrying)))
	{
		playerAnim->Montage_Play(Parrying);

	}
}

void UJW_ParryGuardComponent::guard()
{
	if (!(playerAnim->Montage_IsPlaying(Guard)))
	{
		imguard = true;
		playerAnim->Montage_Play(Guard);

	}
}
void UJW_ParryGuardComponent::guardOff()
{
	if (playerAnim->Montage_IsPlaying(Guard))
	{
		imguard = false;
		playerAnim->Montage_StopWithBlendOut(1.0,Guard);

	}
}


