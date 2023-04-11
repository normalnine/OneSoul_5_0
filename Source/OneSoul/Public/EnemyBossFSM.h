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

	//�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEnemyBossState currState = EEnemyBossState::Idle;

	//Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AOnsSoulPlayer* target;
	//��
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

	//���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString bossName = "Souleater Dimir";

	//����ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currTime = 0;

	//��� ���� ��ٸ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float idleDelayTime = 5.0f;

	//�Ѿ� �� �� �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float traceRange = 5000.0f;

	//���ݹ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackRange = 2500.0f;

	//ȸ�ǹ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float avoidRange  = 150.0f;

	//���� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackDelayTime = 1.5f;

	//�ǰ� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damageDelayTime = 5.0f;

	//ȸ�� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rotateDelayTime = 2.0f;

	//ȸ�� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currrotateDelayTime = 2.0f;

	//��Ʈ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double ghostDistance = 450.0f;

	//��Ʈ ���� Ÿ�̸� �ݺ�/����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSpawnGhost = true;

	//���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currHP;
	//�ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxHP = 5000.0f;
	//���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float accumulatedDamage;
	//���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDamageDealtRecently = false;
	//�׷α� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float groggyDamage = 1000.0f;
	//�ҿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float soul = 150000.0f;
	//�˹� ��ġ
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)S
// 	float enemyAttackForce = 2000.0f;

	//�׾��� �� �������� �ӷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float dieSpeed = 100.0f;
	//�׾��� �� �������� �ϴ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDieMove = false;
	//�׷α� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bGroggy = false;

	//�̵��� �� �ִ� �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float moveRange = 2000.0f;
	//ó�� ��ġ�� ��Ƴ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector originPos;

	//������ ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector randPos;

	//���� ������ Ÿ�̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle DamageResetTimerHandle;

	//��Ʈ ���� Ÿ�̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle GhostHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UOneSoulGameInstance* gameInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* slashedSound;

	

public:
	//���
	UFUNCTION(BlueprintCallable)
	void UpdateIdle();
	//�̵�	
	UFUNCTION(BlueprintCallable)
	void UpdateMove();
	//����
	UFUNCTION(BlueprintCallable)
	void UpdateAttack();
	//���� ���
	UFUNCTION(BlueprintCallable)
	void UpdaetAttackDelay();
	//�ǰ�
	UFUNCTION(BlueprintCallable)
	void UpdateDamaged();
	//����
	UFUNCTION(BlueprintCallable)
	void UpdateDie();
	//����
	UFUNCTION(BlueprintCallable)
	void UpdateReturnPos();

	//���� ����� �ѹ���!!! ȣ�� �Ǵ� �Լ�
	UFUNCTION(BlueprintCallable)
	void ChangeState(EEnemyBossState state);

	//���� �޾��� �� ó���ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float damage);

	//Delay �Լ�
	UFUNCTION(BlueprintCallable)
	bool IsWaitComplete(float delayTime);

	//Ÿ���� �Ѿ� �� �� �ִ�?
	UFUNCTION(BlueprintCallable)
	bool IsTargetTrace();

	//�ش� ��ġ���� �����ϸ� ���¸� Idle �� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void MoveToPos(FVector pos);

	//���� ���� ���ϱ�
	UFUNCTION(BlueprintCallable)
	void DecideAttackPattern();

	//���� ���� �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
	void InitAttackPattern();

	//Fireball ��ȯ
	UFUNCTION(BlueprintCallable)
	void SpawnFireball();

	//FireSpread ��ȯ
	UFUNCTION(BlueprintCallable)
	void SpawnFireSpread();

	//Laser ��ȯ
	UFUNCTION(BlueprintCallable)
	void SpawnLaser();

	//���� ������ �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
	void ResetDamageIfNoRecentDamage();

	//Ghost ��ȯ
	UFUNCTION(BlueprintCallable)
	void SpawnGhost();

	//Roar ����
	UFUNCTION(BlueprintCallable)
	void Roar(float enemyAttackForce);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentOpacity = 1.0f;

	void CameraShake();
};
