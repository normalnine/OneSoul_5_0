// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Magician.h"
#include "Enemy_Magician_FSM.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"
AEnemy_Magician::AEnemy_Magician()
{
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
	StaffMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwoedMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	StaffMeshComp->SetupAttachment(GetMesh(), "Bip01-R-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempStaffMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/Cartoon_Weapon_Pack/FBX/Staff_03.Staff_03'"));
	if (TempStaffMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		StaffMeshComp->SetStaticMesh(TempStaffMesh.Object);

	}
	fsm = CreateDefaultSubobject<UEnemy_Magician_FSM>(TEXT("FSM"));

	HpWidget = CreateDefaultSubobject<UEnemy_HpBar_WidgetComponent>(TEXT("HpBar"));
	HpWidget->SetupAttachment(GetRootComponent());
}