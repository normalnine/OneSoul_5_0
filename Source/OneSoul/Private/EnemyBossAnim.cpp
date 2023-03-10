// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBossAnim.h"
#include "EnemyBoss.h"
//#include "OneSoulCharacter.h"
#include <Components/CapsuleComponent.h>
#include "EnemyBossFSM.h"
#include <Components/SphereComponent.h>

void UEnemyBossAnim::NativeBeginPlay()
{
	//GetOwningActor(); <<-------- Actor 를 가져온다.
	/*AActor* actor = GetOwningActor();
	APawn* p =Cast<APawn>(actor);*/

	//1. 내가 붙어 있는 Pawn 을 가져온다.
	APawn* pawn = TryGetPawnOwner();

	//2. AEnemy 로 형변환한다.
	enemy = Cast<AEnemyBoss>(pawn);
}

void UEnemyBossAnim::AnimNotify_DieEnd()
{
	//3. enemy 가 가지고 있는 fsm 을 이용해서 bDieMove 를 true
	enemy->fsm->bDieMove = true;

	//Cast<AEnemy>(TryGetPawnOwner())->fsm->bDieMove = true;
}

void UEnemyBossAnim::AnimNotify_AttackStartHead()
{
	enemy->capsuleCompHead->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	enemy->capsuleCompHead->SetHiddenInGame(false);
}

void UEnemyBossAnim::AnimNotify_AttackStartLeftHand()
{
	enemy->sphereCompHand_L->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	enemy->sphereCompHand_L->SetHiddenInGame(false);
}

void UEnemyBossAnim::AnimNotify_AttackStartRightHand()
{
	enemy->sphereCompHand_R->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	enemy->sphereCompHand_R->SetHiddenInGame(false);
}

void UEnemyBossAnim::AnimNotify_AttackEndHead()
{
	enemy->capsuleCompHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	enemy->capsuleCompHead->SetHiddenInGame(true);
	enemy->bOverlap = true;
}

void UEnemyBossAnim::AnimNotify_AttackEndLeftHand()
{
	enemy->sphereCompHand_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	enemy->sphereCompHand_L->SetHiddenInGame(true);
	enemy->bOverlap = true;
}

void UEnemyBossAnim::AnimNotify_AttackEndRightHand()
{
	enemy->sphereCompHand_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	enemy->sphereCompHand_R->SetHiddenInGame(true);
	enemy->bOverlap = true;
}

void UEnemyBossAnim::AnimNotify_Fireball()
{
	enemy->fsm->SpawnFireball();
}

void UEnemyBossAnim::AnimNotify_FireSpread()
{
	enemy->fsm->SpawnFireSpread();
}

void UEnemyBossAnim::AnimNotify_Laser()
{
	enemy->fsm->SpawnLaser();
}

void UEnemyBossAnim::AnimNotify_Ghost()
{
	enemy->fsm->SpawnGhost();
}

void UEnemyBossAnim::AnimNotify_Roar()
{
	enemy->fsm->Roar();
}

