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
	//���̷�Ż�޽� ������ �ε�
	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("SkeletalMesh'/Game/LJW/Enemys/Titan/mesh/Titan.Titan'"));
	//������ �ε� �����ϸ�
	if (tempMesh.Succeeded())
	{
		//�������Ҵ�
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		//�޽� ��ġ �� ȸ�� ����
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
		if (target->parrying)
		{
			UE_LOG(LogTemp, Warning, TEXT("parryGood"));
			changeGroggy = true;
		}
		else
		{
			target->ReceiveDamage(1);
			target->DirectionalHitReact(GetActorLocation());
			target->HitReactSounds();
		}


	}

	AShield* shield = Cast<AShield>(otherActor);
	if (shield != nullptr)
	{
		if (player != nullptr)
		{

			//�÷��̾ �˹��Ų��
			FVector imp = -1 * player->GetActorForwardVector() * 3000.0f;
			player->GetCharacterMovement()->AddImpulse(imp, true);

			//�÷��̾��� ��� ����
			player->CurrentStamina = FMath::Clamp(player->CurrentStamina - 20.f, player->MinStamina, player->MaxStamina);

			//�÷��̾��� ��� ȸ�� ���ʵڿ� ȣ���ؾ��ϴµ�
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
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		//FVector HitLocation = SweepResult.Location;
		FName HitLocation = SweepResult.BoneName;
		GetWorld()->SpawnActor<AActor>(HitresultFactory, GetMesh()->GetSocketLocation(TEXT("*HitLocation")), weapon->GetActorRotation() - GetActorRotation(), params);


		/*	FString LocationString = HitLocation.ToString();

			UKismetSystemLibrary::PrintString(this, LocationString, true, true, FLinearColor::Green); */

	}
}
