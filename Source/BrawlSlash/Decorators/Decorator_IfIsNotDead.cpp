// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfIsNotDead.h"

#include "AIController.h"
#include "../Character_EnemyBase.h"

bool UDecorator_IfIsNotDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	return (enemy->state != E_STATE::DEAD);
}