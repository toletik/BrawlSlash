// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "Components/StaticMeshComponent.h"
#include "Character_EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API ACharacter_EnemyBase : public ACharacter_Base
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override {};

public:
	//Base

	ACharacter_EnemyBase() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float walkSpeed{ 600 };

	virtual void TakeHit(int damage) override;
	virtual void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override {};
	virtual void SetAttackState() {};
	FTimerHandle timerHandler;
	bool isRespectingAngularDist{ false };

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "State")
	bool isInInnerCircle{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float initialWaitAttack{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float currentAttackCoolDown{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int attackDamage{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float pushForceAfterBeingHit = 1000.0f;
	UFUNCTION()
	void LookAtPlayer();

	bool beingDashedBack = false;

	bool rotateDashedBack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float rotateSpeedWhenBeingDashedBack{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float timeBeforeRotateWhenBeingDashedBack{ 2.0f };

	UFUNCTION(BlueprintCallable)
	bool IsPlayerInFront(FVector playerPos);

	UFUNCTION(BlueprintImplementableEvent)
	void LastEnemyInGroup();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartFight();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyHitted();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyDeath();
};