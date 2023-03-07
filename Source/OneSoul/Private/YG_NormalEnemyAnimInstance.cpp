// Fill out your copyright notice in the Description page of Project Settings.


#include "YG_NormalEnemyAnimInstance.h"
#include "NormalEnemy_YG.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UYG_NormalEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OneSoulEnemy = Cast<ANormalEnemy_YG>(TryGetPawnOwner());

	if (OneSoulEnemy)
	{
	 OneSoulCharacterMovement = OneSoulEnemy->GetCharacterMovement();
	}

}

void UYG_NormalEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OneSoulEnemy == nullptr)
	{
		OneSoulEnemy = Cast<ANormalEnemy_YG>(TryGetPawnOwner());
	}

	if (OneSoulEnemy)
	{

		//Set Speed
		FVector Velocity{ OneSoulEnemy->GetVelocity() };
		Velocity.Z = 0;
		GroundSpeed = Velocity.Size();

		bIsInAir = OneSoulCharacterMovement->IsFalling();

		FRotator AnimRotation = OneSoulEnemy->GetBaseAimRotation();
		FRotator MovementRotation = OneSoulEnemy->GetActorRotation();

		Roll = UKismetMathLibrary::NormalizedDeltaRotator(
			   AnimRotation,
			   MovementRotation).Roll;
		Pitch = UKismetMathLibrary::NormalizedDeltaRotator(
			    AnimRotation,
			    MovementRotation).Pitch;
		Yaw = UKismetMathLibrary::NormalizedDeltaRotator(
			  AnimRotation,
			  MovementRotation).Yaw;

		FRotator RotationLastTick = OneSoulEnemy->GetActorRotation();
		float Target = UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, OneSoulEnemy->GetActorRotation()).Yaw;

		YawDelta = UKismetMathLibrary::FInterpTo(
			YawDelta,
			(Target / DeltaSeconds) / 7.f,
			DeltaSeconds,
			6.f
		);

		//Set is Accelerating
		if (OneSoulCharacterMovement->GetCurrentAcceleration().Size() > 0)
		{
			bIsAcclerating = true;
		}
		else
		{
			bIsAcclerating = false;
		}

		if (GetCurveValue("FullBody") > 0)
		{
			FullBody = true;
		}
		else
		{
			FullBody = false;
		}

	}
     
}