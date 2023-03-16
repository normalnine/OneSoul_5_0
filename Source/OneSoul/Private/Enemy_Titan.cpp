// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Titan.h"
#include "Enemy_Titan_FSM.h"
#include <Components/CapsuleComponent.h>
#include "OnsSoulPlayer.h"

AEnemy_Titan::AEnemy_Titan()
{
	//���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/Titan/mesh/Titan.Titan'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	LcollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	LcollisionComp->SetupAttachment(GetMesh(), "Wrist_LSocket");

	RcollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldCollision"));
	RcollisionComp->SetupAttachment(GetMesh(), "Wrist_RSocket");

	LcollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Titan::OnOverlapBegin);

	RcollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Titan::OnOverlapBegin);

	fsm = CreateDefaultSubobject<UEnemy_Titan_FSM>(TEXT("FSM"));
}



void AEnemy_Titan::BeginPlay()
{
	Super::BeginPlay();

	RcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AEnemy_Titan::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
	
	if (target != nullptr)
	{
		if (target->parrying)
		{
			UE_LOG(LogTemp, Warning, TEXT("parryGood"));
			changeGroggy = true;
		}
		else
		{
			target->ReceiveDamage(1);
		}


	}
}
