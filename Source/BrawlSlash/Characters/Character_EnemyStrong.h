// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_EnemyBase.h"
#include "Character_EnemyStrong.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API ACharacter_EnemyStrong : public ACharacter_EnemyBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* attackMeshCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* attackMeshStrong;

public:
	ACharacter_EnemyStrong();

	void BeginPlay() override;

	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void SetAttackState() override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownInnerMin { 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownInnerMax{ 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownBackMin{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownBackMax{ 3 };	

	UFUNCTION(BlueprintCallable)
	void BeginAttackCircle();

	UFUNCTION(BlueprintCallable)
	void EndAttackCircle();

	UFUNCTION(BlueprintCallable)
	void BeginAttackStrong();

	UFUNCTION(BlueprintCallable)
	void EndAttackStrong();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool attackStrongProject = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool attackCircleProject = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	bool canRotateDuringStartupCone = false;

	bool isInStartupCone = false;

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartAttackStrong();
};
