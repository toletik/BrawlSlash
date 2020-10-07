// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_MoveToPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->MoveToLocation(OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos"));

	return EBTNodeResult::Succeeded;
}