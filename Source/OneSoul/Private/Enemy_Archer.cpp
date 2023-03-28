// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Archer.h"
#include "Enemy_Archer_FSM.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"

AEnemy_Archer::AEnemy_Archer()
{
	//���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SKELETON.SKELETON'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//Į�޽� ����
	BowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BowMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	BowMeshComp->SetupAttachment(GetMesh(), "Bip01-L-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempBowMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/Cartoon_Weapon_Pack/FBX/Bow.Bow'"));
	if (TempBowMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		BowMeshComp->SetStaticMesh(TempBowMesh.Object);

	}
	fsm = CreateDefaultSubobject<UEnemy_Archer_FSM>(TEXT("FSM"));

	HpWidget = CreateDefaultSubobject<UEnemy_HpBar_WidgetComponent>(TEXT("HpBar"));
	HpWidget->SetupAttachment(GetRootComponent());
}