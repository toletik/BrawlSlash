// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIDirector.h"

#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	radiusFromPlayerToStartFight = Cast<AMyAIController>(enemies[0]->GetController())->radiusBackCircle;
}

// Called every frame
void AMyAIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (enemies.Num() == 0 && enemiesInInner.Num() == 0)
		return;

	if (!isInFight)
		UpdateIfNeedToStartFight();
	else
	{
		UpdateDead();

		UpdateIfIsInInner();

		//UpdateFocus();

		UpdateAttack(DeltaTime);
	}
}


void AMyAIDirector::UpdateIfNeedToStartFight()
{
	for (int i = 0; i <= enemies.Num() - 1; ++i)
		if ((enemies[i]->GetActorLocation() - playerReference->GetActorLocation()).Size() < radiusFromPlayerToStartFight)
		{
			isInFight = true;

			playerReference->isInFight = true;

			for (int j = 0; j <= enemies.Num() - 1; ++j)
				enemies[j]->isInFight = true;

			return;
		}
}

void AMyAIDirector::UpdateDead()
{
	for (int i = enemiesInInner.Num() - 1; i >= 0; --i)
		if (enemiesInInner[i]->state == E_STATE::DEAD)
		{
			if (playerReference->focus == enemiesInInner[i])
				playerReference->focus = nullptr;

			enemiesInInner.RemoveAt(i);
		}
	
	for (int i = enemies.Num() - 1; i >= 0; --i)
		if (enemies[i]->state == E_STATE::DEAD)
		{
			if (playerReference->focus == enemies[i])
				playerReference->focus = nullptr;

			enemies.RemoveAt(i);
		}

	if (enemies.Num() == 0 && enemiesInInner.Num() == 0)
		playerReference->isInFight = false;
}


void AMyAIDirector::UpdateIfIsInInner()
{/*
	for (int i = 0; i <= enemiesInInner.Num() - 1; ++i)
		enemiesInInner[i]->isInInnerCircle = false;

	enemiesInInner.Empty();

	for (int i = 0; i <= enemies.Num() - 1; ++i)
	{
		if (enemiesInInner.Num() < numberOfEnemiesInInner)
			enemiesInInner.Add(enemies[i]);
		else
		{
			float fartestEnemyDist{ 0 };
			ACharacter_EnemyBase* enemyToRemove{ nullptr };

			for (int j = 0; j <= enemiesInInner.Num() - 1; ++j)
				if ( (enemiesInInner[j]->GetActorLocation() - playerReference->GetActorLocation()).Size() > fartestEnemyDist)
				{
					fartestEnemyDist = (enemiesInInner[j]->GetActorLocation() - playerReference->GetActorLocation()).Size();
					enemyToRemove = enemiesInInner[j];
				}

			if ((enemies[i]->GetActorLocation() - playerReference->GetActorLocation()).Size() < fartestEnemyDist)
			{
				enemyToRemove->isInInnerCircle = false;
				enemiesInInner.Remove(enemyToRemove);
				enemies[i]->isInInnerCircle = true;
				enemiesInInner.Add(enemies[i]);

			}

		}

	}*/
	
	
	if (enemiesInInner.Num() < numberOfEnemiesInInner)
		for (int i = enemies.Num() - 1; enemiesInInner.Num() != numberOfEnemiesInInner && i >= 0; --i)
			if (!enemies[i]->isInInnerCircle)
			{
				enemies[i]->isInInnerCircle = true;

				enemiesInInner.Add(enemies[i]);

				enemies.RemoveAt(i);
			}
	
}
/*
void AMyAIDirector::UpdateFocus()
{
	float distanceFromPlayer{ INFINITY };
	ACharacter_EnemyBase* newFocus{ nullptr };

	for (int i = enemiesInInner.Num() - 1; i >= 0; --i)
	{
		if ((enemiesInInner[i]->GetActorLocation() - playerReference->GetActorLocation()).Size() < distanceFromPlayer)
		{
			distanceFromPlayer = (enemiesInInner[i]->GetActorLocation() - playerReference->GetActorLocation()).Size();
			newFocus = enemiesInInner[i];
		}
	}

	if(playerReference->FocusedEnemy != nullptr)
		playerReference->FocusedEnemy->SetIfNeedToGlow(false);

	playerReference->FocusedEnemy = newFocus;

	playerReference->FocusedEnemy->SetIfNeedToGlow(true);

}*/

void AMyAIDirector::UpdateAttack(float DeltaTime)
{
	remainingTimeForNextAttack -= DeltaTime;

	if (remainingTimeForNextAttack <= 0)
	{
		remainingTimeForNextAttack = timeBetweenAttacks;

		ACharacter_EnemyBase* randomEnemy = enemiesInInner[FMath::RandRange(0, enemiesInInner.Num() - 1)];
		
		randomEnemy->SetAttackState();
	}
}

