// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy_Magician_FSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState3 : uint8
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
class ONESOUL_API UEnemy_Magician_FSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemy_Magician_FSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
		EEnemyState3 mState = EEnemyState3::Idle;

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
	void ChangeState(EEnemyState3 state);

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
		class AEnemy_Magician* me;

	UPROPERTY()
		class UEnemy_Magician_anim* anim;


	//���ݹ���
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 1500.0f;

	//�Ѿ� �� �� �ִ� ����
	float traceRange = 2000;

	//���ݴ��ð�
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 hp = 400;

	//ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 maxhp = 400;
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

	//����
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AActor> MazicFactory;

		void mazic();

		//���ϻ���
		void groggy();
		//�����غ�
		UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
			TSubclassOf<class AEnemy_Magician_ReadyAttack> MazicReadyFactory;
		//�ұ�� ����
		UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
			TSubclassOf<class AActor> MazicAttackFactory;

		FVector attackZone;

		int32 mazicattack;

		//�ұ�� ����
		UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
			TSubclassOf<class AActor> MazicAttackEffectFactory;

		UPROPERTY(EditAnywhere)
			class UAnimMontage* AttackMontage;

		UPROPERTY(EditAnywhere)
			class UAnimMontage* damageMontage;

		////ũ��Ƽ�� ������ �ִϸ��̼� ������� �Լ�
		//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//	bool cri = false;

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
		//������ ����
		float allDamage = 0;
		//3�ʵ��ȸ� ���� �������� �ȵǰ�
		bool onedam = true;
		void resetDamage();
		//Į�̶� �浹�Ǹ� ������ �� ����Ʈ
		UPROPERTY(EditDefaultsOnly, Category = HitresultFactory)
			TSubclassOf<class AActor> HitresultFactory;
};
