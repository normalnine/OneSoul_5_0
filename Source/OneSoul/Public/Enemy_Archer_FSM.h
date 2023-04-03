// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_Archer_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState5 : uint8
{
	Idle,
	Move,
	Attack,
	AttackDelay,
	Run,
	Damage,
	Die,
	ReturnPos
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONESOUL_API UEnemy_Archer_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_Archer_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState5 mState = EEnemyState5::Idle;

	//������
	void IdleState();
	//�̵�����
	void MoveState();
	//���ݻ���
	void AttackState();
	//���� ���
	void BackRun();

	//���� ����
	void UpdaetAttackDelay();

	//�ǰݻ���
	void DamageState();
	//��������
	void DieState();
	//����
	void UpdateReturnPos();

	//���� ����� �ѹ���!!! ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemyState5 state);

	//�ǰ� �˸� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess();

	//Ÿ���� �i�ư����ֳ�?
	bool IsTargetTrace();

	//���� �޾��� �� ó���ϴ� �Լ�
	void ReceiveDamage();

	//Delay �Լ�
	bool IsWaitComplete(float delayTime);

	//�ش� ��ġ���� �����ϸ� ���¸� Idle �� ��ȯ�ϴ� �Լ�
	void MoveToPos(FVector pos);

	//���ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 1;
	//����ð�
	float currentTime = 0;

	//����ð�
	float currTime = 0;

	//Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* target;
	//���� ����
	UPROPERTY()
		class AEnemy_Archer* me;

	UPROPERTY()
		class UEnemy_Archer_anim* anim;


	//���ݹ���
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 2000.0f;

	//�Ѿ� �� �� �ִ� ����
	float traceRange = 2500;

	//���ݴ��ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 hp = 30;
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 maxhp = 30;
	//�ǰ� ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 2.0f;
	//�Ʒ��� ������� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float dieSpeed = 50.0f;

	//AI
	UPROPERTY()
		class AAIController* ai;
	//�� ã�� ����� ���� ��ġ
	FVector randomPos;

	//���� ��ġ ��������
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	//ó����ġ�� ��Ƴ��� ����
	FVector originPos;

	//������ ��ġ
	FVector randPos;

	//�̵��Ҽ��ִ� ����
	float moveRange = 2000;


	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AActor> ArrowFactory;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;

	
	void mazic();
	//���ϻ���
	void groggy();

	//�ڷιз����� �Լ�
	UFUNCTION(BlueprintCallable)
		void moveBack();

	//������¿��� �ǰ��� �¾����� true �� �÷��̾����� ������ ��
	bool Hitback = false;

	//�ҿｺ��
	UPROPERTY(EditDefaultsOnly, Category = dropFactory)
		TSubclassOf<class AActor> dropFactory;

	//��� ȿ����
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* SeeplayerSound;

	//�ǰ� ȿ����
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* HITSound;
};

	
