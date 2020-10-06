// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_StopMovement.h"
#include "AIController.h"

EBTNodeResult::Type UTask_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->StopMovement();

	return EBTNodeResult::Succeeded;
}