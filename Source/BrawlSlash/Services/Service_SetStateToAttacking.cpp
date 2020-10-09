// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_SetStateToAttacking.h"

#include "AIController.h"
#include "../Character_EnemyBase.h"

void UService_SetStateToAttacking::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	float ratio = FMath::RandRange(0, 100);

	if (ratio <= enemy->ratioAttackWeak)
		enemy->state = E_STATE::ATTACKING_WEAK;
	else if (ratio <= enemy->ratioAttackWeak + enemy->ratioAttackStrong)
		enemy->state = E_STATE::ATTACKING_STRONG;

}