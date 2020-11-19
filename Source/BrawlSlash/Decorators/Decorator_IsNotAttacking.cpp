// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IsNotAttacking.h"

#include "AIController.h"
#include "../Characters/Character_EnemyStrong.h"

bool UDecorator_IsNotAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemy->state == E_STATE::ATTACKING_STRONG || enemy->state == E_STATE::ATTACKING_WEAK)
	{
		ACharacter_EnemyStrong* strongEnemy = Cast<ACharacter_EnemyStrong>(enemy);
		if (strongEnemy && strongEnemy->isInStartupCone && strongEnemy->canRotateDuringStartupCone)
			return true;

		return false;
	}
	
	return true;
}