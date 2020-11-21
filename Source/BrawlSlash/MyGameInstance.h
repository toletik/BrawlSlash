// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AI/MyAIDirector.h"
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
	bool isMotionBlurActivate{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volumeMusic{ 50.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volumeSFX{ 50.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector lastPlayerPos {0, 0, 0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator lastPlayerRot {0, 0, 0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numberOfPlayerShieldPoints{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool hasRestartLevel {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, bool> allDirectors;

	UFUNCTION(BlueprintCallable)
	void RestartLevelWithInitialValues();
};
