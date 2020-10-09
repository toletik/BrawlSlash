// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorator_NeedToGoFurther.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../Character_EnemyBase.h"

bool UDecorator_NeedToGoFurther::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	FVector enemyPos = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FVector playerPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos");
	FVector playerToSelf = enemyPos - playerPos;

	//get distance
	float distanceFromPlayer = playerToSelf.Size();

	if ((enemy->isInInnerCircle && distanceFromPlayer < OwnerComp.GetBlackboardComponent()->GetValueAsFloat("radiusSmall")) ||
		(!enemy->isInInnerCircle && distanceFromPlayer < OwnerComp.GetBlackboardComponent()->GetValueAsFloat("radiusMedium")))
		return true;

	return false;
}