// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_EnemyWeak.h"
#include "Character_EnemyWeakWithShield.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API ACharacter_EnemyWeakWithShield : public ACharacter_EnemyWeak
{
	GENERATED_BODY()

public:
	ACharacter_EnemyWeakWithShield();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	class UStaticMeshComponent* meshShieldFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	class UStaticMeshComponent* meshShieldBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
	class UComponent_EnemyShield* shieldComponent;
};
