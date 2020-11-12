// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_NeedToGoFurther.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"
#include "../AI/MyAIDirector.h"

bool UDecorator_NeedToGoFurther::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	FVector enemyPos = enemy->GetActorLocation();
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos");

	if ( !enemy->isInInnerCircle && (enemyPos - playerPos).Size() < enemy->currentEnemyGroup->radiusInnerCircle)
		return true;

	return false;
}