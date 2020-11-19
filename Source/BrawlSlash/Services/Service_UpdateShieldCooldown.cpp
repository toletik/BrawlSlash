// Fill out your copyright notice in the Description page of Project Settings.


#include "Service_UpdateShieldCooldown.h"
#include "AIController.h"
#include "../Characters/Character_EnemyBase.h"
#include "../Characters/Component_EnemyShield.h"

void UService_UpdateShieldCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	UComponent_EnemyShield* shield = enemy->FindComponentByClass<UComponent_EnemyShield>();

	if (!shield || !shield->isShieldSwitchable)
		return;

	if (shield->initialWaitShield > 0)
		shield->initialWaitShield -= DeltaSeconds;
	else if (shield->currentShieldTimeActive > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "Active");
		shield->currentShieldTimeActive -= DeltaSeconds;
		if (shield->currentShieldTimeActive <= 0)
		{
			shield->currentShieldTimeToDeActivate = shield->shieldTimeToDeActivate;
			shield->ShieldToDeActivate();
		}
	}

	if (shield->currentShieldTimeToDeActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "ToDeActive");
		shield->currentShieldTimeToDeActivate -= DeltaSeconds;
		if (shield->currentShieldTimeToDeActivate <= 0)
		{
			shield->currentShieldTimeDeActivate = shield->shieldTimeDeActivate;
			shield->ShieldDeActivate();
		}
	}

	if (shield->currentShieldTimeDeActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "DeActive");
		shield->currentShieldTimeDeActivate -= DeltaSeconds;
		if (shield->currentShieldTimeDeActivate <= 0)
		{
			shield->currentShieldTimeToActivate = shield->shieldTimeToActivate;
			shield->ShieldToActivate();
		}
	}

	if (shield->currentShieldTimeToActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "ToActive");
		shield->currentShieldTimeToActivate -= DeltaSeconds;
		if (shield->currentShieldTimeToActivate <= 0)
		{
			shield->currentShieldTimeActive = shield->shieldTimeActive;
			shield->ShieldActivate();
		}
	}
}