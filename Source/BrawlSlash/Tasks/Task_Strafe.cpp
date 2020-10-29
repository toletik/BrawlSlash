// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_Strafe.h"

#include "Engine/World.h"
#include "AIController.h"

EBTNodeResult::Type UTask_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* enemy = OwnerComp.GetAIOwner()->GetPawn();
	FVector enemyPos = enemy->GetActorLocation();

	FHitResult hit;
	FCollisionQueryParams raycastParams;
	raycastParams.AddIgnoredActor(enemy);
	GetWorld()->LineTraceSingleByChannel(hit, enemyPos, enemyPos + enemy->GetActorRightVector() * 200, ECC_Pawn, raycastParams);

	if (hit.Actor == NULL)
		OwnerComp.GetAIOwner()->MoveToLocation(enemyPos + enemy->GetActorRightVector() * 200);
	//else
	//	OwnerComp.GetAIOwner()->MoveToLocation(enemyPos + enemy->GetActorForwardVector() * -200);

	return EBTNodeResult::Succeeded;
}