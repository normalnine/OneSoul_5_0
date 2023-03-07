// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Titan.h"
#include "Enemy_Titan_FSM.h"

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
	fsm = CreateDefaultSubobject<UEnemy_Titan_FSM>(TEXT("FSM"));
}