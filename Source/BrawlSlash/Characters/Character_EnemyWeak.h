// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_EnemyBase.h"
#include "Character_EnemyWeak.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API ACharacter_EnemyWeak : public ACharacter_EnemyBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* attackBox;

public:
	ACharacter_EnemyWeak();

	void BeginPlay() override;

	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void SetAttackState() override;

	void BeginAttack();

	void EndAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownMin{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownMax{ 3 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attack")
	bool needToAttackWeak{ false };

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartNeedAttackWeak();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartAttackWeak();

};
