// Fill out your copyright notice in the Description page of Project Settings.


#include "JW_ParryGuardComponent.h"
#include "OnsSoulPlayer.h"
#include "Components/CapsuleComponent.h"
#include "OneSoulPlayerAnimInstance.h"
#include "Shield.h"


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
		me->imguard = true;
		playerAnim->Montage_Play(Guard);
		/*AShield* shield = Cast<AShield>(GetOwner());
		if (shield !=nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("canblock"));
			shield->shieldcomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}*/

	}
}
void UJW_ParryGuardComponent::guardOff()
{
	if (playerAnim->Montage_IsPlaying(Guard))
	{
		me->imguard = false;
		playerAnim->Montage_StopWithBlendOut(1.0,Guard);
		/*AShield* shield = Cast<AShield>(GetOwner());
		if (shield != nullptr)
		{
			shield->shieldcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}*/

	}
}


