// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfIsInFight.h"

#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"
#include "../AI/MyAIDirector.h"

bool UDecorator_IfIsInFight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!enemy->currentEnemyGroup)
		return false;

	return enemy->currentEnemyGroup->isInFight;
}