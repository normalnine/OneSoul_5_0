// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_Titan_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState4 : uint8
{
	Idle,
	Move,
	MovetoTarget,
	Attack0,
	Attack1,
	Attack2,
	Attack3,
	JumpAttack,
	AttackDelay,
	Damage,
	Die,
	ReturnPos,
	Groggy
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ONESOUL_API UEnemy_Titan_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_Titan_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState4 mState = EEnemyState4::Idle;

	//������
	void IdleState();
	//�̵�����
	void MoveState();
	//Ÿ������ �̵�
	void movetoPlayer();
	//���ݻ���
	void AttackState();
	//���ݻ���
	void AttackState1();
	//���ݻ���
	void AttackState2();
	//���ݻ���
	void AttackState3();
	//���� ���
	void BlockAttack();

	// ���� ����
	void jumpattack();

	//���� ����
	void UpdaetAttackDelay();

	//�ǰݻ���
	void DamageState();
	//��������
	void DieState();
	//����
	void UpdateReturnPos();

	//���� ����� �ѹ���!!! ȣ�� �Ǵ� �Լ�
	void ChangeState(EEnemyState4 state);

	//�ǰ� �˸� �̺�Ʈ �Լ�
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess(float damage);

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
		float idleDelayTime = 2;
	//����ð�
	float currentTime = 0;

	//����ð�
	float currTime = 0;

	//Ÿ��
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* target;
	//���� ����
	UPROPERTY()
		class AEnemy_Titan* me;

	UPROPERTY()
		class UEnemy_Titan_anim* anim;


	//���ݹ���
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	//�Ѿ� �� �� �ִ� ����
	UPROPERTY(EditAnywhere, Category = FSM)
	float traceRange = 3000;

	//���ݴ��ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 1.2f;
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 hp = 2500;
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 maxhp = 2500;
	//�ǰ� ��� �ð�
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 1.0f;
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
	float moveRange = 3000;

	//��ȿ �ѹ��� �ҷ���
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool shout = true;

	//���۾Ƹ� ���¸� ������ٶ�� 
	bool superArmor = false;
		
	//���ϻ���
	void groggy();

	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;

	//������¿��� �ǰ��� �¾����� true �� �÷��̾����� ������ ��
	bool Hitback = false;
	//�������� �� �ϴ� ó�� �ѹ��� �����ϰ� �Ϸ���
	bool jumptotarget =true;

	//�÷��̾� ī�޶� ��鸮�� �Ϸ��� ��ҵ�
	bool camShake = false;
	float currCamShakeTime =0;
	float camShakeTime =1;
	UFUNCTION(BlueprintCallable)
	void Shake();

	//�ѹ� �����Ҷ� �÷��̾� ��ġ�� �ѹ� �����ϴ� ����
	FVector movetarget;

	//����!
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool stopmove = false;


	//�ҿｺ��
	UPROPERTY(EditDefaultsOnly, Category = dropFactory)
		TSubclassOf<class AActor> dropFactory;

	//�ǰ� ȿ����
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* HITSound;
	//Į�̶� �浹�Ǹ� ������ �� ����Ʈ
	UPROPERTY(EditAnywhere, Category = HitresultFactory)
		TSubclassOf<class AActor> HitresultFactory;
	//�ǰ� �� �������� ȿ����
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* StoneSound;

	void resetDamage();
	//������ ����
	float allDamage = 0;
	//3�ʵ��ȸ� ���� �������� �ȵǰ�
	bool onedam = true;

	//�ٷ� �÷��̾ �����Ϸ� �̵�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool movetoattack = false;
};
