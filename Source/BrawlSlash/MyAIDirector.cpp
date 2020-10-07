// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIDirector.h"

// Sets default values
AMyAIDirector::AMyAIDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyAIDirector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyAIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (enemies.Num() == 0)
		return;

	remainingTimeForNextAttack -= DeltaTime;

	if (remainingTimeForNextAttack <= 0)
	{
		remainingTimeForNextAttack = timeBetweenAttacks;

		ACharacter_EnemyBase* randomEnemy = enemies[FMath::RandRange(0, enemies.Num() - 1)];

		//while we pick a dead enemy, remove it from list and if list is empty end test
		while (randomEnemy->state == E_STATE::DEAD)
		{
			enemies.Remove(randomEnemy);

			if (enemies.Num() == 0)
				return;

			randomEnemy = enemies[FMath::RandRange(0, enemies.Num() - 1)];
		}

		randomEnemy->needToAttack = true;
	}
}

