// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character_Player.h"
#include "Character_EnemyBase.h"
#include "MyAIDirector.generated.h"

UCLASS()
class BRAWLSLASH_API AMyAIDirector : public AActor
{
	GENERATED_BODY()

	ACharacter_Player* playerReference {nullptr};

	UPROPERTY(EditAnywhere, category = AI)
	TArray<ACharacter_EnemyBase*> enemies;	
	
	TArray<ACharacter_EnemyBase*> enemiesInInner;

	UPROPERTY(EditAnywhere, category = AI)
	float numberOfEnemiesInInner;

	//UPROPERTY(EditAnywhere, category = AI)
	//float timeBetweenAttacks;

	float remainingTimeForNextAttack;

	bool isInFight{ false };
	bool hasAStrongEnemyInInner{ false };
	float radiusFromPlayerToStartFight{ 0 };


	// Sets default values for this actor's properties
	AMyAIDirector();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	void UpdateIfNeedToStartFight();

	void UpdateDead();

	void UpdateIfIsRespectingAngularDist();

	void SetFocusToClosestEnemy();

	//void UpdateAttack(float DeltaTime);


public:
	void UpdateIfIsInInner();

	UFUNCTION(BlueprintCallable)
	void SetEndToFight();

	bool IsAllEnemiesInCameraSight(APlayerController* controller);

	void SetFocusToNextEnemy();

	void SetFocusToPreviousEnemy();

	void SetDebugFocusToNextEnemy();

	void SetDebugFocusToPreviousEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FRotator rotationForTheFight { 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Angle)
	float angularDisToRespect{ 90 };
};
