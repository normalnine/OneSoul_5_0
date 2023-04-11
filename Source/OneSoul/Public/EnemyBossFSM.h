// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyBossFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyBossState : uint8
{
	Idle,
	Move,
	Attack,
	AttackDelay,
	Damaged,
	Die,
	ReturnPos
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ONESOUL_API UEnemyBossFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemyBossFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	//현재상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEnemyBossState currState = EEnemyBossState::Idle;

	//타겟
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AOnsSoulPlayer* target;
	//나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AEnemyBoss* me;

	//anim instance
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UEnemyBossAnim* anim;

	//anim montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* damageMontage;	

	//anim montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* rotationMontage;

	//ai controller 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AAIController* ai;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemyBossFireball> FireballFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemyBossFireSpread> FireSpreadFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemyBossLaser> LaserFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AEnemyBossGhost> GhostFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UEnemyBossHPBar> EnemyBossHPBarFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UEnemyBossHPBar* EnemyBossHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBossGhost* ghost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UEnemyBossDieUI> EnemyBossDieUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UEnemyBossDieUI* EnemyBossDieUI;

	//보스 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bossName = "Souleater Dimir";

	//현재시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currTime = 0;

	//대기 상태 기다리는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float idleDelayTime = 5.0f;

	//쫓아 갈 수 있는 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float traceRange = 5000.0f;

	//공격범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackRange = 2500.0f;

	//회피범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float avoidRange  = 150.0f;

	//공격 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackDelayTime = 1.5f;

	//피격 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damageDelayTime = 5.0f;

	//회전 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rotateDelayTime = 2.0f;

	//회전 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currrotateDelayTime = 2.0f;

	//고스트 스폰 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double ghostDistance = 450.0f;

	//고스트 스폰 타이머 반복/정지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnGhost = true;

	//현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currHP;
	//최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxHP = 5000.0f;
	//누적 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float accumulatedDamage;
	//누적 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDamageDealtRecently = false;
	//그로기 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float groggyDamage = 1000.0f;
	//소울
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float soul = 150000.0f;
	//넉백 수치
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)S
// 	float enemyAttackForce = 2000.0f;

	//죽었을 때 내려가는 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dieSpeed = 100.0f;
	//죽었을 때 내려가게 하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDieMove = false;
	//그로기 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGroggy = false;

	//이동할 수 있는 반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveRange = 2000.0f;
	//처음 위치를 담아놓을 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector originPos;

	//랜덤한 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector randPos;

	//누적 데미지 타이머
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DamageResetTimerHandle;

	//고스트 스폰 타이머
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle GhostHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOneSoulGameInstance* gameInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* slashedSound;

	

public:
	//대기
	UFUNCTION(BlueprintCallable)
	void UpdateIdle();
	//이동	
	UFUNCTION(BlueprintCallable)
	void UpdateMove();
	//공격
	UFUNCTION(BlueprintCallable)
	void UpdateAttack();
	//공격 대기
	UFUNCTION(BlueprintCallable)
	void UpdaetAttackDelay();
	//피격
	UFUNCTION(BlueprintCallable)
	void UpdateDamaged();
	//죽음
	UFUNCTION(BlueprintCallable)
	void UpdateDie();
	//리턴
	UFUNCTION(BlueprintCallable)
	void UpdateReturnPos();

	//상태 변경시 한번만!!! 호출 되는 함수
	UFUNCTION(BlueprintCallable)
	void ChangeState(EEnemyBossState state);

	//공격 받았을 때 처리하는 함수
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float damage);

	//Delay 함수
	UFUNCTION(BlueprintCallable)
	bool IsWaitComplete(float delayTime);

	//타겟을 쫓아 갈 수 있니?
	UFUNCTION(BlueprintCallable)
	bool IsTargetTrace();

	//해당 위치까지 도착하면 상태를 Idle 로 전환하는 함수
	UFUNCTION(BlueprintCallable)
	void MoveToPos(FVector pos);

	//공격 패턴 정하기
	UFUNCTION(BlueprintCallable)
	void DecideAttackPattern();

	//공격 패턴 초기화
	UFUNCTION(BlueprintCallable)
	void InitAttackPattern();

	//Fireball 소환
	UFUNCTION(BlueprintCallable)
	void SpawnFireball();

	//FireSpread 소환
	UFUNCTION(BlueprintCallable)
	void SpawnFireSpread();

	//Laser 소환
	UFUNCTION(BlueprintCallable)
	void SpawnLaser();

	//누적 데미지 초기화
	UFUNCTION(BlueprintCallable)
	void ResetDamageIfNoRecentDamage();

	//Ghost 소환
	UFUNCTION(BlueprintCallable)
	void SpawnGhost();

	//Roar 공격
	UFUNCTION(BlueprintCallable)
	void Roar(float enemyAttackForce);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentOpacity = 1.0f;

	void CameraShake();
};
