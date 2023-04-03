// Fill out your copyright notice in the Description page of Project Settings.


#include "JW_PlayerRollComponent.h"
#include "OnsSoulPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "OneSoulPlayerAnimInstance.h"



void UJW_PlayerRollComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UJW_PlayerRollComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


    if (!(re))
    {

		//뒤로 밀려나게 해주기
			FVector imp = me->GetActorForwardVector() * 10.0f;
		FVector p = me->GetActorLocation() + imp;
		me->SetActorLocation(p);
      

    }
    if (!ba)
    {
		FVector imp = -1*me->GetActorForwardVector() * 10.0f;
		FVector p = me->GetActorLocation() + imp;
		me->SetActorLocation(p);
    }

}

void UJW_PlayerRollComponent::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	Super::SetupInputBinding(PlayerInputComponent);

	//Space Bar
	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Pressed, this, &UJW_PlayerRollComponent::Roll);
	


}

void UJW_PlayerRollComponent::Roll()
{

	//백스탭 애니메이션이 실행중이 아닐때
 	if (playerAnim->Montage_IsPlaying(BackStep) == false)
 	{
 		//방향키가 안눌리고 있을때 + 공중에 없을때
 		if (me->isMoveF == false && me->isMoveR == false && !(me->GetCharacterMovement()->IsFalling()))
 		{
 			//스테미나가 있을때만 작동하기
 			if (me->CurrentStamina>0)
 			{	//일단 백스탭
 				playerAnim->Montage_Play(BackStep);
                //날라가는 버그때문에 변경
				/*FVector imp = -1 * me->GetActorForwardVector() * 10000.0f;
				me->GetCharacterMovement()->AddImpulse(imp, true);*/
 				//스테미나 감소
                me->CurrentStamina = FMath::Clamp(me->CurrentStamina - 15.f, me->MinStamina, me->MaxStamina);
             
 
 

                //다시 스테미나 채우기
                me->SprintTimer();
                me->RegenerateStamina();
 			}
 			
 		}
 
 
 		// 몽타주가 실행 안 할때  구르기 애니메이션 실행하기 + 공중에 없을때
 		else if (playerAnim->Montage_IsPlaying(Rolling) == false && !(me->GetCharacterMovement()->IsFalling()))
 		{
 			//스테미나가 있을때만 작동하기
 			if (me->CurrentStamina > 0)
 			{
 				//일단구르기
 				playerAnim->Montage_Play(Rolling);
 
 				//스테미나 감소
                me->CurrentStamina = FMath::Clamp(me->CurrentStamina - 15.f, me->MinStamina, me->MaxStamina);
			
 
 				//앞으로 밀려나게 주기
                //날라가는 버그때문에 변경
				/*FVector imp = me->GetActorForwardVector() * 10000.0f;
				me->GetCharacterMovement()->AddImpulse(imp, true);*/
                
               // me->GetCharacterMovement()->AddForce(imp);

				//다시 스테미나 채우기
                me->StopSprint();
				me->RegenerateStamina();
 
 				//구를때 플레이어 충돌끄기 - NoCollision으로 하면 플레이어가 안움직여서 
 				//합칠때 새로운 충돌타입을 만들어서 몬스터랑 안겹치는걸로다가 넣었다가 다시 바꾸기
 				me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
 
 
 
 				//다시 충돌체 활성화 시켜주기
 				FTimerHandle on;
 				GetWorld()->GetTimerManager().SetTimer(on, this, &UJW_PlayerRollComponent::onColl, 0.7f, false);

			
 			}
 			
 		}
 	}
}

//콜리전을 다시 켜주는 함수
void UJW_PlayerRollComponent::onColl()
{
	//원래 출동타입으로 바꿔주기
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	
}

