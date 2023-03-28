// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"
#include <Components/CapsuleComponent.h>
#include "Item.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OnsSoulPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy_Skeleton.h"

AShield::AShield()
{
	shieldcomp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldCapsule"));
	shieldcomp->SetupAttachment(GetRootComponent());
	shieldcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player->imguard)
	{
	shieldcomp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
	shieldcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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
	AEnemy_Skeleton* enemy1 = Cast<AEnemy_Skeleton>(OthrActor);
	if (enemy1 != nullptr)
	{
		//플레이어를 넉백시킨다
		FVector imp = -1 * player->GetActorForwardVector() * 1000.0f;
		player->GetCharacterMovement()->AddImpulse(imp, true);
	}

}
