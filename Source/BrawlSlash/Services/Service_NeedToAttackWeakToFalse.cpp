// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_NeedToAttackWeakToFalse.h"

#include "AIController.h"
#include "../Character_EnemyBase.h"

void UService_NeedToAttackWeakToFalse::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemy->needToAttackWeak = false;
}