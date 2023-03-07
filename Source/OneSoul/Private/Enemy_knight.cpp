// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_knight.h"
#include "Enemy_knight_FSM.h"
#include <Components/SphereComponent.h>

AEnemy_knight::AEnemy_knight()
{
	//���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/Toon_RTS_demo/models/ToonRTS_demo_Knight.ToonRTS_demo_Knight'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}





	fsm = CreateDefaultSubobject<UEnemy_knight_FSM>(TEXT("FSM"));
}