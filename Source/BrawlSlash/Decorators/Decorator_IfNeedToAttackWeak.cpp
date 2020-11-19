// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfNeedToAttackWeak.h"

#include "AIController.h"
#include "../Characters/Character_EnemyWeak.h"

bool UDecorator_IfNeedToAttackWeak::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyWeak* enemy = Cast<ACharacter_EnemyWeak>(OwnerComp.GetAIOwner()->GetPawn());
	if (!enemy)
		return false;
	return enemy->needToAttackWeak;
}