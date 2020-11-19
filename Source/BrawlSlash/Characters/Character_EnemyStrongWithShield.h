// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_EnemyStrong.h"
#include "Character_EnemyStrongWithShield.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API ACharacter_EnemyStrongWithShield : public ACharacter_EnemyStrong
{
	GENERATED_BODY()
	
public:
	ACharacter_EnemyStrongWithShield();

	void TakeHit(int damage) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shield)
	class UComponent_EnemyShield* shieldComponent;
};
