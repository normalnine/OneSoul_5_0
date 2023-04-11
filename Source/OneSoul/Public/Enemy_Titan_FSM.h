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

	//대기상태
	void IdleState();
	//이동상태
	void MoveState();
	//타겟한테 이동
	void movetoPlayer();
	//공격상태
	void AttackState();
	//공격상태
	void AttackState1();
	//공격상태
	void AttackState2();
	//공격상태
	void AttackState3();
	//공격 대기
	void BlockAttack();

	// 점프 공격
	void jumpattack();

	//막기 상태
	void UpdaetAttackDelay();

	//피격상태
	void DamageState();
	//죽음상태
	void DieState();
	//리턴
	void UpdateReturnPos();

	//상태 변경시 한번만!!! 호출 되는 함수
	void ChangeState(EEnemyState4 state);

	//피격 알림 이벤트 함수
	UFUNCTION(BlueprintCallable)
		void OnDamageProcess(float damage);

	//타겠을 쫒아갈수있나?
	bool IsTargetTrace();

	//공격 받았을 때 처리하는 함수
	void ReceiveDamage();

	//Delay 함수
	bool IsWaitComplete(float delayTime);

	//해당 위치까지 도착하면 상태를 Idle 로 전환하는 함수
	void MoveToPos(FVector pos);

	//대기시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float idleDelayTime = 2;
	//경과시간
	float currentTime = 0;

	//현재시간
	float currTime = 0;

	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* target;
	//소유 액터
	UPROPERTY()
		class AEnemy_Titan* me;

	UPROPERTY()
		class UEnemy_Titan_anim* anim;


	//공격범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 150.0f;

	//쫓아 갈 수 있는 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float traceRange = 3000;

	//공격대기시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 1.2f;
	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 hp = 2500;
	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 maxhp = 2500;
	//피격 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 1.0f;
	//아래로 사라지는 속도
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float dieSpeed = 50.0f;

	//AI
	UPROPERTY()
		class AAIController* ai;
	//길 찾기 수행시 랜덤 위치
	FVector randomPos;

	//랜덤 위치 가져오기
	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	//처음위치를 담아놓을 변수
	FVector originPos;

	//랜덤한 위치
	FVector randPos;

	//이동할수있는 범위
	float moveRange = 3000;

	//포효 한번만 할려고
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool shout = true;

	//슈퍼아머 상태를 만들어줄라고 
	bool superArmor = false;
		
	//스턴상태
	void groggy();

	UPROPERTY(EditAnywhere)
		class UAnimMontage* damageMontage;

	//뒤잡상태에서 피격을 맞았을때 true 로 플레이어한테 전달할 값
	bool Hitback = false;
	//점프공격 을 일단 처음 한번만 실행하게 하려고
	bool jumptotarget =true;

	//플레이어 카메라 흔들리게 하려는 요소들
	bool camShake = false;
	float currCamShakeTime =0;
	float camShakeTime =1;
	UFUNCTION(BlueprintCallable)
	void Shake();

	//한번 공격할때 플레이어 위치를 한번 저장하는 벡터
	FVector movetarget;

	//멈춰!
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool stopmove = false;


	//소울스폰
	UPROPERTY(EditDefaultsOnly, Category = dropFactory)
		TSubclassOf<class AActor> dropFactory;

	//피격 효과음
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* HITSound;
	//칼이랑 충돌되면 나오는 피 이펙트
	UPROPERTY(EditAnywhere, Category = HitresultFactory)
		TSubclassOf<class AActor> HitresultFactory;
	//피격 돌 떨어지는 효과음
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		class USoundBase* StoneSound;

	void resetDamage();
	//데미지 누적
	float allDamage = 0;
	//3초동안만 실행 누적실행 안되게
	bool onedam = true;

	//바로 플레이어를 공격하러 이동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool movetoattack = false;
};
