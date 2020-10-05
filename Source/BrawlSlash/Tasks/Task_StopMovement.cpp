// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_StopMovement.h"

EBTNodeResult::Type UTask_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	return EBTNodeResult::Aborted;
}