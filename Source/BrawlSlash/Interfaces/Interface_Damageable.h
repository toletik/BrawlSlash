// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Damageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BRAWLSLASH_API IInterface_Damageable
{
	GENERATED_BODY()

public:

	virtual void TakeHit(int damage) {};
};
