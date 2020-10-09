// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfNotSeePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "Engine/World.h"


bool UDecorator_IfNotSeePlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* enemy = OwnerComp.GetAIOwner()->GetPawn();
	FVector enemyPos = enemy->GetActorLocation();
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos");

	//check if another enemy in sight
	FHitResult hit;
	FCollisionQueryParams raycastParams;
	raycastParams.AddIgnoredActor(enemy);
	GetWorld()->LineTraceSingleByChannel(hit, enemyPos, playerPos, ECC_Pawn, raycastParams);

	if (hit.GetActor() != nullptr && hit.GetActor()->ActorHasTag("Player") )
		return false;

	return true;
}