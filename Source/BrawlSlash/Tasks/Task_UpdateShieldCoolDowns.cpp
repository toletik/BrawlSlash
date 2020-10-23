// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_UpdateShieldCoolDowns.h"

#include "AIController.h"
#include "../Character_EnemyBase.h"
#include "Engine/World.h"

EBTNodeResult::Type UTask_UpdateShieldCoolDowns::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());

	if (enemy->currentShieldTimeActive > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "Active");
		enemy->currentShieldTimeActive -= GetWorld()->GetDeltaSeconds();
		if (enemy->currentShieldTimeActive <= 0)
			enemy->currentShieldTimeToDeActivate = enemy->shieldTimeToDeActivate;
	}

	if (enemy->currentShieldTimeToDeActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "ToDeActive");
		enemy->currentShieldTimeToDeActivate -= GetWorld()->GetDeltaSeconds();
		if (enemy->currentShieldTimeToDeActivate <= 0)
		{
			enemy->currentShieldTimeDeActivate = enemy->shieldTimeDeActivate;
			enemy->ShieldDeActivate();
		}
	}

	if (enemy->currentShieldTimeDeActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "DeActive");
		enemy->currentShieldTimeDeActivate -= GetWorld()->GetDeltaSeconds();
		if (enemy->currentShieldTimeDeActivate <= 0)
			enemy->currentShieldTimeToActivate = enemy->shieldTimeToActivate;
	}

	if (enemy->currentShieldTimeToActivate > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Purple, "ToActive");
		enemy->currentShieldTimeToActivate -= GetWorld()->GetDeltaSeconds();
		if (enemy->currentShieldTimeToActivate <= 0)
		{
			enemy->currentShieldTimeActive = enemy->shieldTimeActive;
			enemy->ShieldActivate();
		}
	}

	return EBTNodeResult::Succeeded;
}