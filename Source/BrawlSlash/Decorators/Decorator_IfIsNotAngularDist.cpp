// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfIsNotAngularDist.h"

#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"

bool UDecorator_IfIsNotAngularDist::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	return (!enemy->isRespectingAngularDist);
}