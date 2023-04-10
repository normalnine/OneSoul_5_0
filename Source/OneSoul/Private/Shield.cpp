// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include <Components/CapsuleComponent.h>
#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy_Skeleton.h"
#include "Enemy_Archer_Arrow.h"
#include "Enemy_Magician_magic.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AShield::AShield()
{
	shieldcomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldCapsule"));
	shieldcomp->SetupAttachment(GetRootComponent());
	shieldcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player->imguard && this != nullptr)
	{
	shieldcomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
	shieldcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//if (reS)
	//{
	///*	UE_LOG(LogTemp, Warning, TEXT("reStamina1"));*/
	//	currTime += GetWorld()->DeltaTimeSeconds;

	//	//스테미나 호출하기
	//	if (currTime > 1)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("reStamina2"));
	//		//플레이어의 기력 회복 호출해야하는데
	//		reStamina();
	//		currTime = 0;
	//		reS = false;

	//	}
	//}
	// 
}

void AShield::BeginPlay()
{
	Super::BeginPlay();
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

	player = Cast<AOnsSoulPlayer>(actor);

	shieldcomp->OnComponentBeginOverlap.AddDynamic(this, &AShield::OnOverlap);
}

void AShield::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	GetItemMesh()->AttachToComponent(InParent, TransformRules, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	if (GetSphereCollision())
	{
		GetSphereCollision()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AShield::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	GetItemMesh()->AttachToComponent(InParent, TransformRules, InSocketName);
	shieldcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShield::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OthrActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	//몬스터를 캐스팅 
	//AEnemy_Skeleton* enemy1 = Cast<AEnemy_Skeleton>(OthrActor);
	//if (enemy1 != nullptr)
	//{
	//	//플레이어를 넉백시킨다
	//	FVector imp = -1 * player->GetActorForwardVector() * 1000.0f;
	//	player->GetCharacterMovement()->AddImpulse(imp, true);

	//}


	AEnemy_Archer_Arrow* arrow = Cast<AEnemy_Archer_Arrow>(OthrActor);
	if (arrow != nullptr)
	{
		//공격을 막고 2초뒤에 기력을 채우기 위해 공격을 막을때마다 시간을 초기화
		FVector imp = -1 * player->GetActorForwardVector() * 500.0f;
		player->GetCharacterMovement()->AddImpulse(imp, true);
		FTimerHandle aaa;
		GetWorld()->GetTimerManager().SetTimer(aaa, this, &AShield::reStamina, 1.0f, false);
		player->Shake();
	}
	AEnemy_Magician_magic* magic = Cast<AEnemy_Magician_magic>(OthrActor);
	if (magic!=nullptr)
	{
		player->Shake();
	}

}

void AShield::noshield()
{	
	//쉴들 해지하려는 함수였는데 방법을 모르겠음
	//player->DetachFromActor(shieldcomp);
}
void AShield::reStamina()
{
	
	player->StopSprint();
	player->RegenerateStamina();
}