// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Interface_Highlightable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInterface_Highlightable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BRAWLSLASH_API IInterface_Highlightable
{
	GENERATED_BODY()


		

public:

	IInterface_Highlightable();

	void SetIfNeedToGlow(bool value);

	UMaterialInstanceDynamic* matDynamic = nullptr;
};
