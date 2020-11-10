// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isXRevert{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isYRevert{ false };


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector lastCheckPointPos {0, 0, 0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator lastCheckPointRot {0, 0, 0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfPlayerShieldPoints{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasTakenACheckpoint {false};
};
