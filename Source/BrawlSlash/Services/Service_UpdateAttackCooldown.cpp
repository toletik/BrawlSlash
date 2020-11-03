// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_UpdateAttackCooldown.h"

#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"
#include "Engine/World.h"

void UService_UpdateAttackCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (enemy->initialWaitAttack > 0)
		enemy->initialWaitAttack -= GetWorld()->GetDeltaSeconds();
	else
	{
		enemy->currentAttackCoolDown -= GetWorld()->GetDeltaSeconds();

		if (enemy->currentAttackCoolDown < 0)
		{
			enemy->SetAttackState();
			if (enemy->isInInnerCircle)
				enemy->currentAttackCoolDown = FMath::RandRange(enemy->attackCoolDownInnerMin, enemy->attackCoolDownInnerMax);
			else
				enemy->currentAttackCoolDown = FMath::RandRange(enemy->attackCoolDownBackMin, enemy->attackCoolDownBackMax);
		}
	}
}