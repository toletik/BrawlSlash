// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_LookAtPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

EBTNodeResult::Type UTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* enemy = OwnerComp.GetAIOwner()->GetPawn();
	FVector selfToPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos") - enemy->GetActorLocation();

	//Setup the lookAt and nullifie the pitch
	FRotator lookAt = FRotationMatrix::MakeFromX(selfToPlayer).Rotator();
	lookAt.Pitch = 0;
	enemy->SetActorRotation(lookAt);

	return EBTNodeResult::Succeeded;
}