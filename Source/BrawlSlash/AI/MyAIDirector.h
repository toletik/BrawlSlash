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

	TArray<ACharacter_EnemyBase*> enemiesInInner;

	UPROPERTY(EditAnywhere, category = AI)
	float numberOfEnemiesToHaveInInner { 0 };

	bool hasAStrongEnemyInInner = false;

	UPROPERTY(EditAnywhere, category = AI)
	bool isTriggerByDistance = true;

	// Sets default values for this actor's properties
	AMyAIDirector();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateIfNeedToStartFight();

	void UpdateIfIsRespectingAngularDist();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = AI)
	TArray<ACharacter_EnemyBase*> enemies;	
	
	void StartFight();

	void SetFocusToClosestEnemy();

	void RemoveEnemy(ACharacter_EnemyBase* enemyToRemove);

	void UpdateIfIsInInner();

	UFUNCTION(BlueprintCallable)
	void SetEndToFight();

	UFUNCTION(BlueprintImplementableEvent)
	void AIDirectorAllEnemiesDead();

	bool IsAllEnemiesInCameraSight(APlayerController* controller);

	AActor* GetNextOrPreviousFocus(bool isGettingNext);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FRotator rotationForTheFight { 0, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = AI)
	float radiusInnerCircle = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = AI)
	float radiusBackCircle = 700.0f;

	UPROPERTY(EditAnywhere, category = AI)
	TArray<ACharacter_EnemyBase*> vipEnemies;

	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	class ULevelSequence* sequenceToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	bool stopPlayerMovementOnSequence = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	bool isSequenceSkippable = true;

	ACharacter_Player* playerReference{ nullptr };

	bool isInFight{ false };

	AActor* dashPointInFight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float minDistanceToAdoptForCamera{ 800.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float maxDistanceToAdoptForCamera{ 1600.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float angularDisToRespect{ 90 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float offsetFromLeft{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float offsetFromRight{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float offsetFromBottom{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float offsetFromTop{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Save)
	int uniqueIndex{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Save)
	bool canClearIndex{ true };
};
