// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_UpdateInvincibleTime.h"

#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"
#include "Engine/World.h"

void UService_UpdateInvincibleTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (enemy->currentInvincibleTime > 0)
		enemy->currentInvincibleTime -= GetWorld()->GetDeltaSeconds();

}