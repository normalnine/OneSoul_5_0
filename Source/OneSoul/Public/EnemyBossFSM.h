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
	UPROPERTY(EditAnywhere)
		class AOnsSoulPlayer* target;
	//나
	UPROPERTY(EditAnywhere)
		class AEnemyBoss* me;

	//anim instance
	UPROPERTY(EditAnywhere)
		class UEnemyBossAnim* anim;

	//anim montage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;	

	//anim montage
	UPROPERTY(EditAnywhere)
		class UAnimMontage* rotationMontage;

	//ai controller 
	UPROPERTY(EditAnywhere)
		class AAIController* ai;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyBossFireball> FireballFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyBossFireSpread> FireSpreadFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyBossLaser> LaserFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AEnemyBossGhost> GhostFactory;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemyBossHPBar> EnemyBossHPBarFactory;

	UPROPERTY(EditAnywhere)
		class UEnemyBossHPBar* EnemyBossHPBar;

	UPROPERTY(EditAnywhere)
	class AEnemyBossGhost* ghost;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UEnemyBossDieUI> EnemyBossDieUIFactory;

	UPROPERTY(EditAnywhere)
		class UEnemyBossDieUI* EnemyBossDieUI;

	//현재시간
	float currTime = 0;
	//대기 상태 기다리는 시간
	float idleDelayTime = 2.0f;

	//쫓아 갈 수 있는 범위
	float traceRange = 10000.0f;

	//공격범위
	float attackRange = 2500.0f;

	//회피범위
	float avoidRange  = 150.0f;

	//공격 대기 시간
	float attackDelayTime = 1.5f;

	//피격 대기 시간
	float damageDelayTime = 5.0f;

	//회전 대기 시간
	float rotateDelayTime = 2.0f;

	//회전 대기 시간
	float currrotateDelayTime = 2.0f;

	//고스트 스폰 위치
	double ghostDistance = 450;

	//고스트 스폰 타이머 반복/정지
	bool bSpawnGhost = true;

	//현재 체력
	float currHP;
	//최대 체력
	float maxHP = 1000;
	//누적 데미지
	float accumulatedDamage;
	//누적 상태
	bool bDamageDealtRecently = false;
	//그로기 데미지
	float groggyDamage = 5000;
	//소울
	float soul = 150000;
	//넉백 수치
	float enemyAttackForce = 2000.0f;

	//죽었을 때 내려가는 속력
	float dieSpeed = 100.0f;
	//죽었을 때 내려가게 하는 변수
	bool bDieMove = false;

	//이동할 수 있는 반경
	float moveRange = 2000.0f;
	//처음 위치를 담아놓을 변수
	FVector originPos;

	//랜덤한 위치
	FVector randPos;

	//누적 데미지 타이머
	FTimerHandle DamageResetTimerHandle;

	//고스트 스폰 타이머
	FTimerHandle GhostHandle;

public:
	//대기
	void UpdateIdle();
	//이동
	void UpdateMove();
	//공격
	void UpdateAttack();
	//공격 대기
	void UpdaetAttackDelay();
	//피격
	void UpdateDamaged();
	//죽음
	void UpdateDie();
	//리턴
	void UpdateReturnPos();

	//상태 변경시 한번만!!! 호출 되는 함수
	void ChangeState(EEnemyBossState state);

	//공격 받았을 때 처리하는 함수
	void ReceiveDamage(float damage);

	//Delay 함수
	bool IsWaitComplete(float delayTime);

	//타겟을 쫓아 갈 수 있니?
	bool IsTargetTrace();

	//해당 위치까지 도착하면 상태를 Idle 로 전환하는 함수
	void MoveToPos(FVector pos);

	//공격 패턴 정하기
	void DecideAttackPattern();

	//공격 패턴 초기화
	void InitAttackPattern();

	//Fireball 소환
	void SpawnFireball();

	//FireSpread 소환
	void SpawnFireSpread();

	//Laser 소환
	void SpawnLaser();

	//누적 데미지 초기화
	void ResetDamageIfNoRecentDamage();

	//Ghost 소환
	void SpawnGhost();

	//Roar 공격
	void Roar();
};
