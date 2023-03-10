// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_knight.h"
#include "Enemy_knight_FSM.h"
#include "OnsSoulPlayer.h"
#include <Components/SphereComponent.h>
#include <Components/CapsuleComponent.h>


AEnemy_knight::AEnemy_knight()
{
	//스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/Toon_RTS_demo/models/ToonRTS_demo_Knight.ToonRTS_demo_Knight'"));
	//데이터 로드 성공하면
	if (tempMesh.Succeeded())
	{
		//데이터할당
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//메시 위치 및 회전 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	SwordCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollisionComp->SetupAttachment(GetMesh(),"Bip001-R-HandSocket");

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollision"));
	collisionComp->SetupAttachment(GetMesh(), "Bip001-L-HandSocket");


	SwordCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_knight::OnOverlapBeginsword);

	fsm = CreateDefaultSubobject<UEnemy_knight_FSM>(TEXT("FSM"));
}

void AEnemy_knight::OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
	//UEFSM* enemy = Cast<UEFSM>(otherActor);
	if (target != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OverLap 1"));


	}
}