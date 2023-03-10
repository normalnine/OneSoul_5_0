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
	//�÷��̾� �ִϸ��̼� �ҷ�����
	UOneSoulPlayerAnimInstance* playerAnim = Cast<UOneSoulPlayerAnimInstance>(me->GetMesh()->GetAnimInstance());

	//�̵��ϰ��������� �����ϱ�

	

	// ��Ÿ�ְ� ���� �� �Ҷ�  �����ϱ�
	if (playerAnim->Montage_IsPlaying(Rolling) == false)
	{
	
		//�ϴܱ�����
		playerAnim->Montage_Play(Rolling);

		//������ �÷��̾� �浹���� - NoCollision���� �ϸ� �÷��̾ �ȿ������� ��ĥ�� ���ο� �浹Ÿ���� ���� ���Ͷ� �Ȱ�ġ�°ɷδٰ� �־��ٰ� �ٽ� �ٲٱ�
		//me->GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);



		//�ٽ� �浹ü Ȱ��ȭ �����ֱ�
		FTimerHandle on;
		GetWorld()->GetTimerManager().SetTimer(on,this,&UJW_PlayerRollComponent::onColl,0.5f,false);
	}
	
	

}

//�ݸ����� �ٽ� ���ִ� �Լ�
void UJW_PlayerRollComponent::onColl()
{
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
