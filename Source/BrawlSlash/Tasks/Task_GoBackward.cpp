// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GoBackward.h"

#include "AIController.h"

EBTNodeResult::Type UTask_GoBackward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* enemy = OwnerComp.GetAIOwner()->GetPawn();

	OwnerComp.GetAIOwner()->MoveToLocation(enemy->GetActorLocation() - enemy->GetActorForwardVector() * 50);

	return EBTNodeResult::Succeeded;
}