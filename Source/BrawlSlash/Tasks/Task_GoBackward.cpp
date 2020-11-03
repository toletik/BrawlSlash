// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_GoBackward.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UTask_GoBackward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* enemy = OwnerComp.GetAIOwner()->GetPawn();
	FVector enemyPos = enemy->GetActorLocation();
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos");
	FVector selfToPlayer = playerPos - enemyPos;
	selfToPlayer.Normalize();

	OwnerComp.GetAIOwner()->MoveToLocation(enemyPos - selfToPlayer * 200);

	return EBTNodeResult::Succeeded;
}