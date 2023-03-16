// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_FSM.h"
#include "OnsSoulPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JW_ParryGuardComponent.h"
#include "Enemy_Skeleton_Anim.h"
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>


AEnemy_Skeleton::AEnemy_Skeleton()
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
	SwordMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwoedMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	SwordMeshComp->SetupAttachment(GetMesh(), "Bip01-R-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempSworodMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SWORD_W_sword_01.SWORD_W_sword_01'"));
	if (TempSworodMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		SwordMeshComp->SetStaticMesh(TempSworodMesh.Object);

	}

	//���� �޽� ����
	ShieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMeshComp"));

	//���̷��� �ȿ� �ٿ��ֱ�
	ShieldMeshComp->SetupAttachment(GetMesh(), "Bip01-L-Hand");

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempShieldMesh(TEXT("StaticMesh'/Game/LJW/Enemys/SimpleSkeleton/mesh/SHIELD.SHIELD'"));
	if (TempShieldMesh.Succeeded())
	{
		//4-4.���̷�Ż�޽� �������Ҵ�
		ShieldMeshComp->SetStaticMesh(TempShieldMesh.Object);

	}


	SwordCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	SwordCollisionComp->SetupAttachment(SwordMeshComp);

	collisionComp =CreateDefaultSubobject<USphereComponent>(TEXT("ShieldCollision"));
	collisionComp->SetupAttachment(ShieldMeshComp);


	SwordCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Skeleton::OnOverlapBeginsword);


	fsm = CreateDefaultSubobject<UEnemy_Skeleton_FSM>(TEXT("FSM"));
}

void AEnemy_Skeleton::BeginPlay()
{
	Super::BeginPlay();

	SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy_Skeleton::OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{

// 		AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
// 		
// 		if (target != nullptr)
// 		{
// 			if (target->parrying)
// 			{
// 				UE_LOG(LogTemp, Warning, TEXT("parryGood"));
// 				changeGroggy = true;
// 			}
// 			else
// 			{
// 				//�÷��̾ �������϶�
// 				if (target->compPlayerGuard->imguard)
// 				{
// 					
// 					FString sectionName = FString::Printf(TEXT("thing"));
// 					PlayAnimMontage(fsm->damageMontage, 1.0f, FName(*sectionName));
// 
// 					//�÷��̾ �˹��Ų��
// 					FVector imp = -1 * target->GetActorForwardVector() * 1000.0f;
// 					target->GetCharacterMovement()->AddImpulse(imp, true);
// 
// 					//�÷��̾��� ��� ����
// 					target->CurrentStamina = FMath::Clamp(target->CurrentStamina - 10.f, target->MinStamina, target->MaxStamina);
// 				}
// 				else
// 				{
// 					target->ReceiveDamage(1);
// 				}
// 
// 			}
// 		
// 
// 		}
	}