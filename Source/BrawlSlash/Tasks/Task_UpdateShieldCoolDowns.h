// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_UpdateShieldCoolDowns.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API UTask_UpdateShieldCoolDowns : public UBTTaskNode
{
	GENERATED_BODY()

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
