// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ComboIfNeedToOrToIdle.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API UAnimNotify_ComboIfNeedToOrToIdle : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
