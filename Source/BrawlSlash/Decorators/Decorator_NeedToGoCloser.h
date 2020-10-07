// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Decorator_NeedToGoCloser.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API UDecorator_NeedToGoCloser : public UBTDecorator
{
	GENERATED_BODY()
	
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
