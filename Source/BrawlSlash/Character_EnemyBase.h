// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "Character_EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API ACharacter_EnemyBase : public ACharacter_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* attackCircle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeHit(int damage) override;

public:
	// Sets default values for this character's properties
	ACharacter_EnemyBase();

	UFUNCTION(BlueprintCallable)
	void BeginAttackCircle();

	UFUNCTION(BlueprintCallable)
	void EndAttackCircle();

	void SetAttackState();

	float remainingTimeStun{ 0.f };
	float remainingTimeRecovery{ 0.f };

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "State")
	bool isInInnerCircle{ false };

	bool needToAttackWeak{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	float ratioAttackWeak{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	float ratioAttackStrong{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	int weakDamage{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	int strongDamage{ 2 };
};


