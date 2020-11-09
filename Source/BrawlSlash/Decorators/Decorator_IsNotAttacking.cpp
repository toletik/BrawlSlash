// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IsNotAttacking.h"

#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"

bool UDecorator_IsNotAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemy->state == E_STATE::ATTACKING_STRONG || enemy->state == E_STATE::ATTACKING_WEAK)
	{
		if (enemy->isInStartupCone && enemy->canRotateDuringStartupCone)
			return true;

		return false;
	}
	
	return true;
}