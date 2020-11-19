// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_IfIsShieldSwitchable.h"

#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"
#include "../Characters/Component_EnemyShield.h"

bool UDecorator_IfIsShieldSwitchable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	UComponent_EnemyShield* shield = enemy->FindComponentByClass<UComponent_EnemyShield>();

	if (!shield)
		return false;

	return shield->isShieldSwitchable;
}