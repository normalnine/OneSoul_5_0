// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Archer.h"
#include "Enemy_Archer_FSM.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"

AEnemy_Archer::AEnemy_Archer()
{
	//스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SKELETON.SKELETON'"));
	//데이터 로드 성공하면
	if (tempMesh.Succeeded())
	{
		//데이터할당
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//메시 위치 및 회전 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
	//칼메시 설정
	BowMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BowMeshComp"));

	//스켈레톤 팔에 붙여주기
	BowMeshComp->SetupAttachment(GetMesh(), "Bip01-L-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempBowMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/Cartoon_Weapon_Pack/FBX/Bow.Bow'"));
	if (TempBowMesh.Succeeded())
	{
		//4-4.스켈레탈메시 데이터할당
		BowMeshComp->SetStaticMesh(TempBowMesh.Object);

	}
	fsm = CreateDefaultSubobject<UEnemy_Archer_FSM>(TEXT("FSM"));

	HpWidget = CreateDefaultSubobject<UEnemy_HpBar_WidgetComponent>(TEXT("HpBar"));
	HpWidget->SetupAttachment(GetRootComponent());
}