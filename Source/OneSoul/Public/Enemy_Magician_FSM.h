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

	//대기상태
	void IdleState();
	//이동상태
	void MoveState();
	//공격상태
	void AttackState();
	//공격 대기
	void BackRun();

	//막기 상태
	void UpdaetAttackDelay();

	//피격상태
	void DamageState();
	//죽음상태
	void DieState();
	//리턴
	void UpdateReturnPos();

	//상태 변경시 한번만!!! 호출 되는 함수
	void ChangeState(EEnemyState3 state);

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
		float idleDelayTime = 1;
	//경과시간
	float currentTime = 0;

	//현재시간
	float currTime = 0;

	//타깃
	UPROPERTY(VisibleAnywhere, Category = FSM)
		class AOnsSoulPlayer* target;
	//소유 액터
	UPROPERTY()
		class AEnemy_Magician* me;

	UPROPERTY()
		class UEnemy_Magician_anim* anim;


	//공격범위
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackRange = 1500.0f;

	//쫓아 갈 수 있는 범위
	float traceRange = 2000;

	//공격대기시간
	UPROPERTY(EditAnywhere, Category = FSM)
		float attackDelayTime = 2.0f;
	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 hp = 400;

	//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FSM)
		int32 maxhp = 400;
	//피격 대기 시간
	UPROPERTY(EditDefaultsOnly, Category = FSM)
		float damageDelayTime = 3.0f;
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
	float moveRange = 2000;

	//공격
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		TSubclassOf<class AActor> MazicFactory;

		void mazic();

		//스턴상태
		void groggy();
		//공격준비
		UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
			TSubclassOf<class AEnemy_Magician_ReadyAttack> MazicReadyFactory;
		//불기둥 공격
		UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
			TSubclassOf<class AActor> MazicAttackFactory;

		FVector attackZone;

		int32 mazicattack;

		//불기둥 공격
		UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
			TSubclassOf<class AActor> MazicAttackEffectFactory;

		UPROPERTY(EditAnywhere)
			class UAnimMontage* AttackMontage;

		UPROPERTY(EditAnywhere)
			class UAnimMontage* damageMontage;

		////크리티컬 데미지 애니메이션 재생여부 함수
		//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//	bool cri = false;

		//뒤로밀려가는 함수
		UFUNCTION(BlueprintCallable)
			void moveBack();

		//뒤잡상태에서 피격을 맞았을때 true 로 플레이어한테 전달할 값
		bool Hitback = false;

		//소울스폰
		UPROPERTY(EditDefaultsOnly, Category = dropFactory)
			TSubclassOf<class AActor> dropFactory;

		//사망 효과음
		UPROPERTY(EditDefaultsOnly, Category = Sound)
			class USoundBase* SeeplayerSound;

		//피격 효과음
		UPROPERTY(EditDefaultsOnly, Category = Sound)
			class USoundBase* HITSound;
		//데미지 누적
		float allDamage = 0;
		//3초동안만 실행 누적실행 안되게
		bool onedam = true;
		void resetDamage();
		//칼이랑 충돌되면 나오는 피 이펙트
		UPROPERTY(EditDefaultsOnly, Category = HitresultFactory)
			TSubclassOf<class AActor> HitresultFactory;
};
