// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Character_EnemyBase.h"
#include "MyAIDirector.generated.h"

UCLASS()
class BRAWLSLASH_API AMyAIDirector : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = AI)
	TArray<ACharacter_EnemyBase*> enemies;	
	

	TArray<ACharacter_EnemyBase*> enemiesInInner;

	UPROPERTY(EditAnywhere, category = AI)
	float numberOfEnemiesInInner;

	float currentNumberOfEnemiesInInner{0};

	UPROPERTY(EditAnywhere, category = AI)
	float timeBetweenAttacks;

	float remainingTimeForNextAttack;

	// Sets default values for this actor's properties
	AMyAIDirector();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateDead();

	void UpdatePosition();

	void UpdateAttack(float DeltaTime);

};
