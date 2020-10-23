// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfIsShieldSwitchable.h"

#include "AIController.h"
#include "../Character_EnemyBase.h"

bool UDecorator_IfIsShieldSwitchable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	return enemy->isShieldSwitchable;
}