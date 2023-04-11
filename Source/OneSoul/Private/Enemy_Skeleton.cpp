// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Skeleton.h"
#include "Enemy_Skeleton_FSM.h"
#include "OnsSoulPlayer.h"
#include "Enemy_HpBar.h"
#include "Weapon.h"
#include "AIController.h"
#include "Enemy_HpBar_WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JW_ParryGuardComponent.h"
#include "JW_PlayerBaseComponent.h"
#include "OneSoulPlayerAnimInstance.h"
#include "Enemy_Skeleton_Anim.h"
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Engine/EngineTypes.h"
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/Actor.h>
#include <Components/BoxComponent.h>
#include "Shield.h"



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

	collisionComp =CreateDefaultSubobject<UCapsuleComponent>(TEXT("ShieldCollision"));
	collisionComp->SetupAttachment(ShieldMeshComp);


	SwordCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Skeleton::OnOverlapBeginsword);

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Skeleton::OnOverlapBeginshield);


	fsm = CreateDefaultSubobject<UEnemy_Skeleton_FSM>(TEXT("FSM"));



	HpWidget = CreateDefaultSubobject<UEnemy_HpBar_WidgetComponent>(TEXT("HpBar"));
	HpWidget->SetupAttachment(GetRootComponent());

	//몬스터 캡슐에 칼이 맞으면
	UCapsuleComponent* mycapsule = GetCapsuleComponent();
	mycapsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Skeleton::OnOverlapME);

	ConstructorHelpers::FObjectFinder<USoundBase> tempbSound(TEXT("SoundWave'/Game/LJW/Enemys/sound/070_Equip_10.070_Equip_10'"));
	if (tempbSound.Succeeded())
	{
		blockSound = tempbSound.Object;
	}
	SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy_Skeleton::BeginPlay()
{
	Super::BeginPlay();

	HpWidget->UpdateCurrHP(fsm->hp, fsm->maxhp);

	
}

void AEnemy_Skeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (fsm->isShield)
	{
	collisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEnemy_Skeleton::OnOverlapBeginsword(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{

 		AOnsSoulPlayer* target = Cast<AOnsSoulPlayer>(otherActor);
 		
 		if (target != nullptr)
 		{
 			if (target->parrying)
 			{
				//FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator();
				//while (It)
				//{
				//	APlayerController* Pcon = It->Get();
				//	// 가져온 플레이어 컨트롤러 객체로 처리
				//	++It;
				//	player->EnableInput(Pcon);
				//}
				SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//효과음 발생
				UGameplayStatics::PlaySound2D(GetWorld(), blockSound);
				changeGroggy = true;
				FActorSpawnParameters params;
				params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				//이펙트 발생
				GetWorld()->SpawnActor<AActor>(effectfactory1, target->GetMesh()->GetSocketTransform("ring_01_l"), params);
			}
 			else
 			{	if(target->Health>1)
			{
				SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				//플레이어 데미지 호출하는함수
				UGameplayStatics::ApplyDamage(target, 40, fsm->ai, this, UDamageType::StaticClass());
				//플레이어를 넉백시킨다
				FVector imp = -1 * target->GetActorForwardVector() * 500.0f;
				target->GetCharacterMovement()->AddImpulse(imp, true);
				//플레이어 카메라 흔들리기
				//player->Shake();
				}
			}
 		
 
 		}
		AShield* shield = Cast<AShield>(otherActor);
		if (shield !=nullptr)
		{
			if (!(fsm->superArm))
			{
			
			FString sectionName = FString::Printf(TEXT("thing"));
			PlayAnimMontage(fsm->damageMontage, 1.0f, FName(*sectionName));
			//칼 방패 충돌체 끄기
			SwordCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			collisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//효과음 발생
			UGameplayStatics::PlaySound2D(GetWorld(), blockSound);

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			//튕겨지는 이펙트 생성
			GetWorld()->SpawnActor<AActor>(effectfactory,shield->GetActorTransform(),params);
			
			
			auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

			player = Cast<AOnsSoulPlayer>(actor);

				if (player !=nullptr)
				{
				
					//플레이어를 넉백시킨다
					FVector imp = -1 * player->GetActorForwardVector() * 3000.0f;
					player->GetCharacterMovement()->AddImpulse(imp, true);

					//플레이어 카메라 흔들리기
					player->Shake();

					//플레이어의 기력 감소
					player->CurrentStamina = FMath::Clamp(player->CurrentStamina - 10.f, player->MinStamina, player->MaxStamina);

					//플레이어의 기력 회복 몇초뒤에 호출해야하는데
					FTimerHandle ddd;
					GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Skeleton::reStamina, 2.0f, false);
				}
			}
			
		}
}

void AEnemy_Skeleton::OnOverlapBeginshield(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWeapon* weapon = Cast<AWeapon>(otherActor);
	if (weapon != nullptr)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		//튕겨지는 이펙트 생성
		GetWorld()->SpawnActor<AActor>(effectfactory,weapon->GetActorTransform(), params);
		
		//피격호출안되도록 먼저 막기
		one=true;
		
		//방패에 맞으면 몸의 콜리전을 잠시 끄고 다시 키는거 - 방패 맞고 몸맞아서 따블로 맞으면 안되니까
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		//효과음 발생
		UGameplayStatics::PlaySound2D(GetWorld(), blockSound);
	
		//몽타주실행
		FString sectionName = FString::Printf(TEXT("Block"));
		PlayAnimMontage(fsm->damageMontage, 1.0f, FName(*sectionName));
		//스테이트 변경
		fsm->mState = EEnemyState1::Shield;
		//플레이어 불러오기
		auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());
		player = Cast<AOnsSoulPlayer>(actor);
		//플레이어 카메라 흔들리기
		player->Shake();
		//플레이어가 방패를 때려서 반동 애니메이션 재생
		player->compPlayerBase->SAR();
		//플레이어 칼 콜리전 끄기
		weapon->WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//플레이어 뒤로 넉백시키기
		FVector imp = player->GetActorForwardVector() * -3000.0f;
		player->GetCharacterMovement()->AddImpulse(imp, true);
		//공격안되는 버그있어서 다시 공격하려면
		player->IsAttacking=false;
		

		//1초 뒤에 몸의 콜리전을 키는 함수호출
		FTimerHandle ddd;
		GetWorld()->GetTimerManager().SetTimer(ddd, this, &AEnemy_Skeleton::oncoll, 1.5f, false);
	}
}

void AEnemy_Skeleton::reStamina()
{
	player->SprintTimer();
	player->RegenerateStamina();
}

void AEnemy_Skeleton::OnOverlapME(class UPrimitiveComponent* selfComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWeapon* weapon = Cast<AWeapon>(otherActor);
	if (weapon != nullptr)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


		//FVector HitLocation = SweepResult.Location;
		/*FName HitLocation = SweepResult.BoneName;
		GetWorld()->SpawnActor<AActor>(HitresultFactory, GetMesh()->GetSocketLocation(TEXT("*HitLocation")),weapon->GetActorRotation()-GetActorRotation(),params);*/
		
		fsm->OnDamageProcess(weapon->randDamage);
		/*	FString LocationString = HitLocation.ToString();`

			UKismetSystemLibrary::PrintString(this, LocationString, true, true, FLinearColor::Green); */
	
	}
}

void AEnemy_Skeleton::oncoll()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	one=false;
}
