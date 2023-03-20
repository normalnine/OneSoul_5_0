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
   // if (re)
   // {
   //     if (me->CurrentStamina>99)
   //     {
			////�ٽ� ���׹̳� ä���
			//me->RegenerateStamina();
   //     }
		
   // }
	
}

void UJW_PlayerRollComponent::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	Super::SetupInputBinding(PlayerInputComponent);

	//Space Bar
	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Released, this, &UJW_PlayerRollComponent::Roll);
	


}

void UJW_PlayerRollComponent::Roll()
{
	
	//�������� �ƴҶ�
	/*if (true)
	{

	}*/
	//�齺�� �ִϸ��̼��� �������� �ƴҶ�
 	if (playerAnim->Montage_IsPlaying(BackStep) == false)
 	{
 		//����Ű�� �ȴ����� ������ + ���߿� ������
 		if (me->isMoveF == false && me->isMoveR == false && !(me->GetCharacterMovement()->IsFalling()))
 		{
 			//���׹̳��� �������� �۵��ϱ�
 			if (me->CurrentStamina>0)
 			{	//�ϴ� �齺��
 				playerAnim->Montage_Play(BackStep);
 
 				//���׹̳� ����
                me->CurrentStamina = FMath::Clamp(me->CurrentStamina - 15.f, me->MinStamina, me->MaxStamina);
             
 
 
 				//�ڷ� �з����� ���ֱ�
 				FVector imp = -1 * me->GetActorForwardVector() * 10000.0f;
 				me->GetCharacterMovement()->AddImpulse(imp, true);

                //�ٽ� ���׹̳� ä���
                me->SprintTimer();
                me->RegenerateStamina();
 			}
 			
 		}
 
 
 		// ��Ÿ�ְ� ���� �� �Ҷ�  ������ �ִϸ��̼� �����ϱ� + ���߿� ������
 		else if (playerAnim->Montage_IsPlaying(Rolling) == false && !(me->GetCharacterMovement()->IsFalling()))
 		{
 			//���׹̳��� �������� �۵��ϱ�
 			if (me->CurrentStamina > 0)
 			{
 				//�ϴܱ�����
 				playerAnim->Montage_Play(Rolling);
 
 				//���׹̳� ����
                me->CurrentStamina = FMath::Clamp(me->CurrentStamina - 15.f, me->MinStamina, me->MaxStamina);
			
 
 				//������ �з����� �ֱ�
 				FVector imp = me->GetActorForwardVector() * 10000.0f;
 				me->GetCharacterMovement()->AddImpulse(imp, true);
 
				//�ٽ� ���׹̳� ä���
                me->StopSprint();
				me->RegenerateStamina();
 
 				//������ �÷��̾� �浹���� - NoCollision���� �ϸ� �÷��̾ �ȿ������� 
 				//��ĥ�� ���ο� �浹Ÿ���� ���� ���Ͷ� �Ȱ�ġ�°ɷδٰ� �־��ٰ� �ٽ� �ٲٱ�
 				me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
 
 
 
 				//�ٽ� �浹ü Ȱ��ȭ �����ֱ�
 				FTimerHandle on;
 				GetWorld()->GetTimerManager().SetTimer(on, this, &UJW_PlayerRollComponent::onColl, 0.7f, false);

			
 			}
 			
 		}
 	}
}

//�ݸ����� �ٽ� ���ִ� �Լ�
void UJW_PlayerRollComponent::onColl()
{
	//���� �⵿Ÿ������ �ٲ��ֱ�
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	
}

