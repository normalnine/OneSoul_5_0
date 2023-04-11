// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Titan.h"
#include "Enemy_Titan_FSM.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"
#include <Components/CapsuleComponent.h>
#include "OnsSoulPlayer.h"
#include "Shield.h"
#include "Weapon.h"

AEnemy_Titan::AEnemy_Titan()
{
	//스켈레탈메시 데이터 로드
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/Titan/mesh/Titan.Titan'"));
	//데이터 로드 성공하면
	if (tempMesh.Succeeded())
	{
		//데이터할당
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//메시 위치 및 회전 설정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}


	LcollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SwordCollision"));
	LcollisionComp->SetupAttachment(GetMesh(), "Wrist_LSocket");

	RcollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldCollision"));
	RcollisionComp->SetupAttachment(GetMesh(), "Wrist_RSocket");

	LcollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Titan::OnOverlapBegin);

	RcollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Titan::OnOverlapBegin);

	fsm = CreateDefaultSubobject<UEnemy_Titan_FSM>(TEXT("FSM"));

	HpWidget = CreateDefaultSubobject<UEnemy_HpBar_WidgetComponent>(TEXT("HpBar"));
	HpWidget->SetupAttachment(GetRootComponent());
}



void AEnemy_Titan::BeginPlay()
{
	Super::BeginPlay();

	RcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

	player = Cast<AOnsSoulPlayer>(actor);


}

void AEnemy_Titan::OnOverlapBegin(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);

	if (target != nullptr)
	{
		
			if (target->Health > 1)
			{
				//플레이어 데미지 호출하는함수
				UGameplayStatics::ApplyDamage(target, 70, fsm->ai, this, UDamageType::StaticClass());
				//플레이어를 넉백시킨다
				RcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

				LcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				FVector imp = -1 * target->GetActorForwardVector() * 2000.0f;
				target->GetCharacterMovement()->AddImpulse(imp, true);
			}

	}

	AShield* shield = Cast<AShield>(otherActor);
	if (shield != nullptr)
	{
		if (player != nullptr)
		{

			//플레이어를 넉백시킨다
			FVector imp = -1 * player->GetActorForwardVector() * 3000.0f;
			player->GetCharacterMovement()->AddImpulse(imp, true);

			//플레이어의 기력 감소
			player->CurrentStamina = FMath::Clamp(player->CurrentStamina - 20.f, player->MinStamina, player->MaxStamina);

			//플레이어의 기력 회복 몇초뒤에 호출해야하는데
			FTimerHandle ddd;
			GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Titan::reStamina, 1.0f, false);
		}
	}

}
void AEnemy_Titan::reStamina()
{
	player->StopSprint();
	player->SprintTimer();
	player->RegenerateStamina();
}

void AEnemy_Titan::OnOverlapME(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWeapon* weapon = Cast<AWeapon>(otherActor);
	if (weapon != nullptr)
	{
		

		UE_LOG(LogTemp, Warning, TEXT("titan"));
		/*	FString LocationString = HitLocation.ToString();

			UKismetSystemLibrary::PrintString(this, LocationString, true, true, FLinearColor::Green); */

	}
}
