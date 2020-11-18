// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_UpdateShieldCooldown.h"
#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"

void UService_UpdateShieldCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (!enemy->isShieldSwitchable)
		return;

	if (enemy->initialWaitShield > 0)
		enemy->initialWaitShield -= DeltaSeconds;
	else if (enemy->currentShieldTimeActive > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "Active");
		enemy->currentShieldTimeActive -= DeltaSeconds;
		if (enemy->currentShieldTimeActive <= 0)
		{
			enemy->currentShieldTimeToDeActivate = enemy->shieldTimeToDeActivate;
			enemy->ShieldToDeActivate();
		}
	}

	if (enemy->currentShieldTimeToDeActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "ToDeActive");
		enemy->currentShieldTimeToDeActivate -= DeltaSeconds;
		if (enemy->currentShieldTimeToDeActivate <= 0)
		{
			enemy->currentShieldTimeDeActivate = enemy->shieldTimeDeActivate;
			enemy->ShieldDeActivate();
		}
	}

	if (enemy->currentShieldTimeDeActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "DeActive");
		enemy->currentShieldTimeDeActivate -= DeltaSeconds;
		if (enemy->currentShieldTimeDeActivate <= 0)
		{
			enemy->currentShieldTimeToActivate = enemy->shieldTimeToActivate;
			enemy->ShieldToActivate();
		}
	}

	if (enemy->currentShieldTimeToActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "ToActive");
		enemy->currentShieldTimeToActivate -= DeltaSeconds;
		if (enemy->currentShieldTimeToActivate <= 0)
		{
			enemy->currentShieldTimeActive = enemy->shieldTimeActive;
			enemy->ShieldActivate();
		}
	}
}