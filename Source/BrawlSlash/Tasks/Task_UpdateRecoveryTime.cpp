// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_UpdateRecoveryTime.h"

#include "AIController.h"
#include "../Character_EnemyBase.h"
#include "Engine/World.h"

EBTNodeResult::Type UTask_UpdateRecoveryTime::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	enemy->remainingTimeRecovery -= GetWorld()->GetDeltaSeconds();

	return EBTNodeResult::Succeeded;
}