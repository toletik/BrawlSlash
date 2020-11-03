// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_NeedToGoCloser.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"

bool UDecorator_NeedToGoCloser::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	FVector enemyPos = enemy->GetActorLocation();
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos");

	//get distance
	float distanceFromPlayer = (enemyPos - playerPos).Size();

	if ((enemy->isInInnerCircle && distanceFromPlayer > OwnerComp.GetBlackboardComponent()->GetValueAsFloat("radiusInnerCircle")) ||
		(!enemy->isInInnerCircle && distanceFromPlayer > OwnerComp.GetBlackboardComponent()->GetValueAsFloat("radiusBackCircle")))
		return true;

	return false;
}