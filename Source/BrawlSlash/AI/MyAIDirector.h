// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Characters/Character_Player.h"
#include "../Characters/Character_EnemyBase.h"
#include "MyAIDirector.generated.h"

UCLASS()
class BRAWLSLASH_API AMyAIDirector : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = AI, meta = (AllowPrivateAccess = "true"))
	TArray<ACharacter_EnemyBase*> enemies;	
	
	TArray<ACharacter_EnemyBase*> enemiesInInner;

	UPROPERTY(EditAnywhere, category = AI)
	float numberOfEnemiesToHaveInInner { 0 };

	//UPROPERTY(EditAnywhere, category = AI)
	//float timeBetweenAttacks;

	//float remainingTimeForNextAttack;

	bool hasAStrongEnemyInInner{ false };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float radiusFromPlayerToStartFight{ 0 };

	// Sets default values for this actor's properties
	AMyAIDirector();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void UpdateIfNeedToStartFight();

	void UpdateIfIsRespectingAngularDist();

	void SetFocusToClosestEnemy();

	//void UpdateAttack(float DeltaTime);


public:
	void RemoveEnemy(ACharacter_EnemyBase* enemyToRemove);

	void UpdateIfIsInInner();

	UFUNCTION(BlueprintCallable)
	void SetEndToFight();

	UFUNCTION(BlueprintImplementableEvent)
	void AIDirectorAllEnemiesDead();

	bool IsAllEnemiesInCameraSight(APlayerController* controller);

	void SetFocusToNextEnemy();

	void SetFocusToPreviousEnemy();

	void SetDebugFocusToNextEnemy();

	void SetDebugFocusToPreviousEnemy();

	UPROPERTY(EditAnywhere, category = AI)
	TArray<ACharacter_EnemyBase*> vipEnemies;

	UPROPERTY(EditAnywhere, category = AI)
	AActor* actorInWhichCallCustomEventOnVipEnemiesDeath {nullptr};

	ACharacter_Player* playerReference{ nullptr };

	bool isInFight{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FRotator rotationForTheFight { 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Angle)
	float angularDisToRespect{ 90 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Angle)
	float offsetFromLeft{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Angle)
	float offsetFromRight{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Angle)
	float offsetFromBottom{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Angle)
	float offsetFromTop{0};
};
