// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_Skeleton_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState1 : uint8
{
	Idle,
	Move,
	Attack,
	Attackcombo1,
	Attackcombo2,
	Attackcombo3,
	AttackDelay,
	Shield,
	Damage,
	Die,
	ReturnPos,
	Groggy
};
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ONESOUL_API UEnemy_Skeleton_FSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEnemy_Skeleton_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState1 mState = EEnemyState1::Idle;

	//������
	void IdleState();
	//�̵�����
	void MoveState();
	//���ݻ���
	void AttackState();
	//�޺�����1
	void AttackCombo1();
	//�޺�����2
	void AttackCombo2();
	//�޺�����3
	void AttackCombo3();

	//���� ���
	void BlockAttack();

	//���� ����
	void UpdaetAttackDelay();

	//�ǰݻ���
	void DamageState();
	//��������
	void DieState();
	//����
	void UpdateReturnPos();

	//���� ����� �ѹ���!!! ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemyState1 state);

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
		class AEnemy_Skeleton* me;

	UPROPERTY()
		class UEnemy_Skeleton_Anim* anim;


	//���ݹ���
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 120.0f;

	//�Ѿ� �� �� �ִ� ����
	float traceRange = 1000;

	//���ݴ��ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 1.0f;
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 hp = 10;
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 maxhp = 10;
	//�ǰ� ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 3.0f;
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

	//��� ������
	bool isShield = false;

	//�׷α� ����
	void groggy();

	//ũ��Ƽ�� ������ �ִϸ��̼� ������� �Լ�
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool cri = false;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;

	//�ǰݴ����������� �� �����ϵ��� �ϴ°�
	bool fight = false;

	int32 index =0;

	//�ڷιз����� �Լ�
	UFUNCTION(BlueprintCallable)
	void moveBack();

	//������¿��� �ǰ��� �¾����� true �� �÷��̾����� ������ ��
	bool Hitback = false;

	//�ҿｺ��
	UPROPERTY(EditDefaultsOnly, Category = dropFactory)
		TSubclassOf<class AActor> dropFactory;

	void showHP();
	//���۾Ƹӻ���
	bool superArm = false;

	//�޺� ������ �Ҷ����� ������ ���ư��� �Լ� (��Ʈ����� �� �ȸ���)
	void gof();

	int32 timerCount = 0;
	FTimerHandle ddd;

	//��� ȿ����
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* SeeplayerSound;
	//�ǰ� ȿ����
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* HITSound;

};
