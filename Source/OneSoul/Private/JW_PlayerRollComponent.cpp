// Fill out your copyright notice in the Description page of Project Settings.


#include "JW_PlayerRollComponent.h"
#include "OnsSoulPlayer.h"
#include "Components/CapsuleComponent.h"
#include "OneSoulPlayerAnimInstance.h"

void UJW_PlayerRollComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UJW_PlayerRollComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UJW_PlayerRollComponent::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	Super::SetupInputBinding(PlayerInputComponent);

	//Space Bar
	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Released, this, &UJW_PlayerRollComponent::Roll);


}

void UJW_PlayerRollComponent::Roll()
{
	//플레이어 애니메이션 불러오기
	UOneSoulPlayerAnimInstance* playerAnim = Cast<UOneSoulPlayerAnimInstance>(me->GetMesh()->GetAnimInstance());

	//이동하고있을때만 실행하기

	

	// 몽타주가 실행 안 할때  실행하기
	if (playerAnim->Montage_IsPlaying(Rolling) == false)
	{
	
		//일단구르기
		playerAnim->Montage_Play(Rolling);

		//구를때 플레이어 충돌끄기 - NoCollision으로 하면 플레이어가 안움직여서 합칠때 새로운 충돌타입을 만들어서 몬스터랑 안겹치는걸로다가 넣었다가 다시 바꾸기
		//me->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);



		//다시 충돌체 활성화 시켜주기
		FTimerHandle on;
		GetWorld()->GetTimerManager().SetTimer(on,this,&UJW_PlayerRollComponent::onColl,0.5f,false);
	}
	
	

}

//콜리전을 다시 켜주는 함수
void UJW_PlayerRollComponent::onColl()
{
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
