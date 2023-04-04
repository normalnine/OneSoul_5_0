// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Titan_FSM.h"
#include "Enemy_Titan.h"
#include "Enemy_Titan_anim.h"
#include "Enemy_HpBar.h"
#include "Enemy_HpBar_WidgetComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include <NavigationSystem.h>
#include <Kismet/KismetMathLibrary.h>
#include "OnsSoulPlayer.h"
#include <GameFramework/Character.h>
// Sets default values for this component's properties
UEnemy_Titan_FSM::UEnemy_Titan_FSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/LJW/Enemys/Titan/anim/AM_Enemy_Titan.AM_Enemy_Titan'"));
	if (tempMontage.Succeeded())
	{
		damageMontage = tempMontage.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundBase> tempHITSound(TEXT("SoundWave'/Game/LJW/Enemys/sound/zombie10.zombie10'"));
	if (tempHITSound.Succeeded())
	{
		HITSound = tempHITSound.Object;
	}
}


// Called when the game starts
void UEnemy_Titan_FSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AOnsSoulPlayer::StaticClass());

	target = Cast<AOnsSoulPlayer>(actor);

	me = Cast<AEnemy_Titan>(GetOwner());

	anim = Cast<UEnemy_Titan_anim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	originPos = me->GetActorLocation();

	me->HpWidget->UpdateCurrHP(hp, maxhp);
	
}


// Called every frame
void UEnemy_Titan_FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState4::Idle:
		IdleState();
		break;
	case EEnemyState4::Move:
		MoveState();
		break;
	case EEnemyState4::MovetoTarget:
		movetoPlayer();
		break;
	case EEnemyState4::Attack0:
		AttackState();
		break;
	case EEnemyState4::Attack1:
		AttackState1();
		break;
	case EEnemyState4::Attack2:
		AttackState2();
		break;
	case EEnemyState4::Attack3:
		AttackState3();
		break;
	case EEnemyState4::JumpAttack:
		jumpattack();
		break;
	case EEnemyState4::AttackDelay:
		UpdaetAttackDelay();
		break;
	case EEnemyState4::Damage:
		DamageState();
		break;
	case EEnemyState4::Die:
		DieState();
		break;
	case EEnemyState4::ReturnPos:
		UpdateReturnPos();
		break;
	case EEnemyState4::Groggy:
		UpdateReturnPos();
		break;
	}

	FVector Ddir = target->GetActorLocation() - me->GetActorLocation();
	FVector DdirSize = Ddir;
	Ddir.Normalize();
	float Ddotvalue = FVector::DotProduct(me->GetActorLocation(), Ddir);
	float Dangle = UKismetMathLibrary::DegAcos(Ddotvalue);
	//�־����� ü�¹پȺ��̵��� ����
	if (DdirSize.Size() > 2000)
	{
		me->HpWidget->SetVisibility(false);
	}
	//�ް����� Ȯ���ϴ°�
	if (Dangle > 155 && DdirSize.Size() < 500)
	{
		Hitback = true;
	}
	else
	{
		Hitback = false;
	}
	//�÷��̾� ī�޶� ��鸮�� ȿ��
	if (camShake == true)
	{
		//2. ����ð��� �帣�� �ϰ�
		currCamShakeTime += DeltaTime;
		//3. ���࿡ ����ð��� ���ؽð����� ������
		if (currCamShakeTime < camShakeTime)
		{
			//4. ī�޶� �����ϰ� ��ġ��Ű��
			float randY = FMath::RandRange(-5.0f, 5.0f);
			float randZ = FMath::RandRange(-5.0f, 5.0f);
			target->Camera->SetRelativeLocation(FVector(0, randY, randZ));
		}
		//5. �׷��� ������ �ʱ�ȭ(����ð�, bFire, ī�޶���ġ)
		else
		{
			currCamShakeTime = 0;
			camShake = false;
			target->Camera->SetRelativeLocation(FVector::ZeroVector);
		}
	}
}
void UEnemy_Titan_FSM::IdleState()
{
	
	superArmor=false;

	//�þ߿� ������ �����̱� ����
	if (IsTargetTrace())
	{	//UE_LOG(LogTemp, Warning, TEXT("I_SEE"));
		if (jumptotarget)
		{
			//UE_LOG(LogTemp, Warning, TEXT("JUMPTOYOU"));
			ChangeState(EEnemyState4::JumpAttack);
			jumptotarget=false;
			
			
		}
		//ChangeState(EEnemyState4::Move);
	}

	//else
	//{
	//	//idleDelayTime �� ������	
	//	if (IsWaitComplete(idleDelayTime))
	//	{
	//		//������¸� Move �� �Ѵ�.
	//		ChangeState(EEnemyState4::Move);
	//	}
	//}
}
void UEnemy_Titan_FSM::MoveState()
{
	// �þ߿� ���Դ��� ����
	bool bTrace = IsTargetTrace();

	//Ÿ���� ���ϴ� ������ ���ϰ�
	FVector dir = target->GetActorLocation() - me->GetActorLocation();

	//ó�� ��ġ, ���� ���� ��ġ�� �Ÿ�
	float dist = FVector::Distance(originPos, me->GetActorLocation());


	//�þ߿� ���Դٸ�
	if (bTrace)
	{
		
		//���࿡ target - me �Ÿ��� ���ݹ������� ������
		if (dir.Length() < attackRange)
		{
			
			int32 index = FMath::RandRange(0, 1);
			if (index == 0)
			{
				ChangeState(EEnemyState4::Attack0);
			}
			else
			{
				ChangeState(EEnemyState4::Attack1);
			}
		}
		//�׷��� ������
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("movetotarget"));
			ai->MoveToLocation(target->GetActorLocation());
		}
	}
	//�þ߿� ������ �ʾҴٸ�
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("movetorandpos"));
		// ������ ��ġ���� ������ �� Idle ���·� ��ȯ
		MoveToPos(randPos);
	}
}

void UEnemy_Titan_FSM::movetoPlayer()
{
	ai->MoveToLocation(target->GetActorLocation());

	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	//���࿡ target - me �Ÿ��� ���ݹ������� ������
	if (dir.Length() < attackRange)
	{

		int32 index = FMath::RandRange(0, 1);
		if (index == 0)
		{
			ChangeState(EEnemyState4::Attack0);
		}
		else
		{
			ChangeState(EEnemyState4::Attack1);
		}
	}
}

void UEnemy_Titan_FSM::AttackState()
{
	//���Ͱ� �÷��̾� �������� �����ϵ��� �ϴ� �� ����
	FVector des = target->GetActorLocation();
	FVector dir = des - me->GetActorLocation();
	FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//�ִϸ��̼ǿ��� ���� Ű�°ɷ� ����
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	if (me->changeGroggy)
	{
		me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ChangeState(EEnemyState4::Groggy);
	}
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}
}

void UEnemy_Titan_FSM::AttackState1()
{
	superArmor = true;
	//���Ͱ� �÷��̾� �������� �����ϵ��� �ϴ� ��
	//FVector des = target->GetActorLocation();
	//FVector dir = des - me->GetActorLocation();
	//FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//�ִϸ��̼ǿ��� ���� Ű�°ɷ� ����
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}


}
void UEnemy_Titan_FSM::AttackState2()
{
	superArmor=true;

	//���Ͱ� �÷��̾� �������� �����ϵ��� �ϴ� ��
	//FVector des = target->GetActorLocation();
	//FVector dir = des - me->GetActorLocation();
	//FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//�ִϸ��̼ǿ��� ���� Ű�°ɷ� ����
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}


}
void UEnemy_Titan_FSM::AttackState3()
{
	superArmor = true;

	//���Ͱ� �÷��̾� �������� �����ϵ��� �ϴ� ��
	//FVector des = target->GetActorLocation();
	//FVector dir = des - me->GetActorLocation();
	//FRotator dirx = dir.Rotation();
	//me->SetActorRotation(dirx);

	//�ִϸ��̼ǿ��� ���� Ű�°ɷ� ����
	/*me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);*/

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > 2)
	{

		currTime = 0;

		ChangeState(EEnemyState4::AttackDelay);
	}


}

void UEnemy_Titan_FSM::jumpattack()
{
	// �̵��ϴ� ��ƾ ȣ��
	ai->MoveToLocation(movetarget);

	// ���� ����
	float JumpHeight = 500.0f;
	float JumpTime = FMath::Sqrt(2 * JumpHeight / GetWorld()->GetGravityZ());
	float JumpSpeed = JumpHeight / JumpTime;
	FVector JumpVelocity = FVector(0, 0, JumpSpeed);
	me->GetCharacterMovement()->Launch(JumpVelocity);
	currTime += GetWorld()->DeltaTimeSeconds;
	if (stopmove)
	{
	ai->StopMovement();
		if (currTime > 4)
		{
			currTime = 0;
			ChangeState(EEnemyState4::MovetoTarget);
		}
	}
	
}

void UEnemy_Titan_FSM::UpdaetAttackDelay()
{	
	superArmor=true;
	//���� �浹 �ݸ��� ���°�
	me->RcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	me->LcollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (IsWaitComplete(attackDelayTime))
	{
		
		FVector dir = target->GetActorLocation() - me->GetActorLocation();
		float dist = dir.Length();

		if (dist < attackRange)
		{
			//�⺻ ���� ���� 2��
			int32 index = FMath::RandRange(0, 1);
			//hp�� �������� �������� �߰�
			if (hp<11)
			{index =FMath::RandRange(0,2);}

			//�������� ����
			if (index == 0)
			{ChangeState(EEnemyState4::Attack1);}
			else if(index ==1)
			{ChangeState(EEnemyState4::Attack2);}
			else if(index ==2)
			{ChangeState(EEnemyState4::Attack3);}
			
		}
		else
		{

			ChangeState(EEnemyState4::Move);
		}
	}
}
void UEnemy_Titan_FSM::DamageState()
{
	//damageDelayTime �� ������
	if (IsWaitComplete(damageDelayTime))
	{
		//Move ����
		ChangeState(EEnemyState4::Move);
	}
}
void UEnemy_Titan_FSM::DieState()
{
	me->HpWidget->SetVisibility(false);
	//���� ���� �ִϸ��̼��� ������ �ʾҴٸ�
	//�ٴڳ������� �ʵ��� ó��
	/*if (anim->bDieDone == false)
	{
		return;
	}*/

	//��� �Ʒ��� �������� �ʹ�.
	//���Ӥ���� ���� ��=����+����Ƽ
	//FVector p0 = me->GetActorLocation();
	//FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	//FVector p = p0 + vt;
	////2. ���࿡ p.Z �� -200 ���� ������ �ı��Ѵ�
	//if (p.Z < -200)
	//{
	//	me->Destroy();
	//}
	//3. �׷��� ������ �ش� ��ġ�� �����Ѵ�
	//else
	//{
	//	me->SetActorLocation(p);
	//}


}
void UEnemy_Titan_FSM::UpdateReturnPos()
{
	MoveToPos(originPos);
}
void UEnemy_Titan_FSM::OnDamageProcess()
{
	me->HpWidget->SetVisibility(true);
	hp--;
	me->HpWidget->UpdateCurrHP(hp, maxhp);
	
		//ü���� �����ִٸ�
		if (hp > 0)
		{

			//ü���� �����̸� 2������ ����
			if (hp<10 && shout == true)
			{	
				UE_LOG(LogTemp, Warning, TEXT("2page"));
				//��Ÿ�ֽ���
				FString sectionName = FString::Printf(TEXT("Shout"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				//���۾Ƹӻ��·� ����
				superArmor=true;
				//�ѹ��� �����Ϸ��� ������ �ٲ�
				shout=false;
				mState = EEnemyState4::AttackDelay;
			}
			//�ް����� �¾�����
			if (Hitback&&!superArmor)
			{
				//��Ÿ�ֽ���
				FString sectionName = FString::Printf(TEXT("Cri"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				//���º���
				mState = EEnemyState4::Damage;
			}
			//���۾Ƹ� ���°� �ƴҶ�, �� �Ϲ� �ǰݸ��
			if (!(superArmor))
			{
				//ȿ�������
				UGameplayStatics::PlaySound2D(GetWorld(), HITSound);
				FString sectionName = FString::Printf(TEXT("Damage0"));
				me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
				FVector imp = target->GetActorForwardVector() * 500.0f;
				me->GetCharacterMovement()->AddImpulse(imp, true);
			//���¸� �ǰ����� ��ȯ
			mState = EEnemyState4::Damage;
			currentTime = 0;
			}
		}

		else
		{
			//���¸� �������� ��ȯ
			mState = EEnemyState4::Die;
			//ĸ�� �浹ü ��Ȱ��ȭ
			me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			//���� �ִϸ��̼� ���
			FString sectionName = FString::Printf(TEXT("Die"));
			me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
			GetWorld()->SpawnActor<AActor>(dropFactory, me->GetActorTransform());
		}
		//�ִϸ��̼� ���� ����ȭ
		anim->animState = mState;
		ai->StopMovement();
	
		
}



bool UEnemy_Titan_FSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	dest = loc.Location;
	return result;
}

void UEnemy_Titan_FSM::groggy()
{
	UE_LOG(LogTemp, Warning, TEXT("groggy"));
	anim->animState = mState;

	me->changeGroggy = false;

	ai->StopMovement();
	
	ChangeState(EEnemyState4::Idle);

}

void UEnemy_Titan_FSM::Shake()
{
		camShake = true;
}

bool UEnemy_Titan_FSM::IsTargetTrace()
{


	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	FVector dirSize = dir;
	dir.Normalize();


	FVector na = me->GetActorLocation();
	float dotvalue = FVector::DotProduct(na, dir);

	//acos
	float angle = UKismetMathLibrary::DegAcos(dotvalue);

	//���� ���� 40���� �۰� ���� �÷��̾���� �Ÿ��� ������ �Ÿ����� ������
	if (angle < 90 && dirSize.Size() < traceRange)
	{


		FHitResult hitInfo;
		FCollisionQueryParams param;
		param.AddIgnoredActor(me);
		bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, me->GetActorLocation(), target->GetActorLocation(), ECollisionChannel::ECC_EngineTraceChannel4, param);

		if (bhit)
		{
			if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")))
			{
				return true;
			}

		}

	}

	return false;

}

bool UEnemy_Titan_FSM::IsWaitComplete(float delayTime)
{

	currTime += GetWorld()->DeltaTimeSeconds;


	if (currTime > delayTime)
	{

		currTime = 0;

		return true;
	}


	return false;
}

void UEnemy_Titan_FSM::ChangeState(EEnemyState4 state)
{
	//���� ���� �α׸� �������
	//UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	//if (enumPtr != nullptr)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("%s -----> %s"),
	//		*enumPtr->GetNameStringByIndex((int32)mState),
	//		*enumPtr->GetNameStringByIndex((int32)state));
	//}

	//���� ���¸� ����
	mState = state;

	//anim �� ���� ����
	anim->animState = state;

	//���� �ð� �ʱ�ȭ
	currTime = 0;

	//ai �� ������ ������
	ai->StopMovement();

	//���¿� ���� �ʱ⼳��
	switch (mState)
	{
	case EEnemyState4::Attack0:
		//currTime = attackDelayTime;
		break;
	case EEnemyState4::Move:
	{	
		superArmor=false;

		//�׺���̼� �ý��� ��������
		UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		//������ ��ġ�� �����
		FNavLocation loc;
		ns->GetRandomReachablePointInRadius(originPos, 1000, loc);
		randPos = loc.Location;
	}
	break;

	case EEnemyState4::JumpAttack:
	{
		movetarget = target->GetActorLocation();
		/*FTimerHandle aaa;
		GetWorld()->GetTimerManager().SetTimer(aaa,this,&UEnemy_Titan_FSM::Shake,2,false);*/
	}
	break;

	case EEnemyState4::Damage:
	{
		//1. ������ ���� �̴´� (0, 1 ��)
		int32 rand = FMath::RandRange(0, 1);
		//2. Damage0, Damage1 �̶� ���ڿ��� �����.
		FString sectionName = FString::Printf(TEXT("Damage%d"), rand);
		//3. ��Ÿ�ָ� �÷����Ѵ�.
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(*sectionName));
	}
	break;
	case EEnemyState4::Die:

		//�浹�ȵǰ� ����
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);




		//Die ��Ÿ�� ����
		//me->PlayAnimMontage(damageMontage, 1.0f, FName(TEXT("Die")));

		break;
	}
}
void UEnemy_Titan_FSM::ReceiveDamage()
{
	//�Ǹ� ������
	hp--;
	//hp �� 0���� ũ�� Damage ���·� ��ȯ
	if (hp > 0)
	{
		ChangeState(EEnemyState4::Damage);
	}
	//�׷��� ������ Die ���·� ��ȯ
	else
	{

		ChangeState(EEnemyState4::Die);
	}
}

void UEnemy_Titan_FSM::MoveToPos(FVector pos)
{
	//�ش� ��ġ(pos) �� ����
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(pos);

	//���࿡ �������� �����ߴٸ�
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		//Idle ���·� ��ȯ
		ChangeState(EEnemyState4::Idle);
	}
}
