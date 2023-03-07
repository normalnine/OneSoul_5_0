// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_FSM.h"
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>


AEnemy_Skeleton::AEnemy_Skeleton()
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
	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwoedMeshComp"));

	//스켈레톤 팔에 붙여주기
	SwordMeshComp->SetupAttachment(GetMesh(), "Bip01-R-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSworodMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SWORD_W_sword_01.SWORD_W_sword_01'"));
	if (TempSworodMesh.Succeeded())
	{
		//4-4.스켈레탈메시 데이터할당
		SwordMeshComp->SetStaticMesh(TempSworodMesh.Object);

	}

	//방패 메시 설정
	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMeshComp"));

	//스켈레톤 팔에 붙여주기
	ShieldMeshComp->SetupAttachment(GetMesh(), "Bip01-L-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempShieldMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SHIELD.SHIELD'"));
	if (TempShieldMesh.Succeeded())
	{
		//4-4.스켈레탈메시 데이터할당
		ShieldMeshComp->SetStaticMesh(TempShieldMesh.Object);

	}


	SwordCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollisionComp->SetupAttachment(SwordMeshComp);

	collisionComp =CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollision"));
	collisionComp->SetupAttachment(ShieldMeshComp);



	fsm = CreateDefaultSubobject<UEnemy_Skeleton_FSM>(TEXT("FSM"));
}
