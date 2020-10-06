// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfIsCloseToPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

bool UDecorator_IfIsCloseToPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	FVector enemyPos = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos");
	FVector playerToSelf = enemyPos - playerPos;

	//get distance
	float distanceFromPlayer = playerToSelf.Size();

	return (distanceFromPlayer > OwnerComp.GetBlackboardComponent()->GetValueAsFloat("rangeCloseToPlayer"));

}