// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIDirector.h"

#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"

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

	if (enemies.Num() == 0)
		return;

	if (!isInFight)
		UpdateIfNeedToStartFight();
	else
	{
		UpdateDead();

		UpdateAttack(DeltaTime);
	}
}


void AMyAIDirector::UpdateIfNeedToStartFight()
{
	for (int i = 0; i <= enemies.Num() - 1; ++i)
	{
		if ((enemies[i]->GetActorLocation() - playerReference->GetActorLocation()).Size() < radiusFromPlayerToStartFight)
		{
			isInFight = true;

			for (int j = 0; j <= enemies.Num() - 1; ++j)
				enemies[j]->isInFight = true;

			UpdateIfIsInInner();


			AMyAIController* tempController = Cast<AMyAIController>(enemies[i]->GetController());

			playerReference->isInFight = true;
			playerReference->currentEnemyGroup = this;
			playerReference->focus = enemies[i];
			playerReference->SetCameraStatsFight();
			playerReference->debugInnerCircleRadius = tempController->radiusInnerCircle;
			playerReference->debugBackCircleRadius = tempController->radiusBackCircle;
			return;
		}
	}
}

void AMyAIDirector::UpdateDead()
{
	for (int i = enemies.Num() - 1; i >= 0; --i)
	{
		if (enemies[i]->state == E_STATE::DEAD)
		{
			bool needToSetNewFocus = false;

			if (playerReference->focus == enemies[i])
				needToSetNewFocus = true;

			enemies.RemoveAt(i);

			if (needToSetNewFocus)
				SetFocusToClosestEnemy();

			UpdateIfIsInInner();
		}
	}

	if (enemies.Num() == 0)
	{
		playerReference->isInFight = false;
		playerReference->currentEnemyGroup = nullptr;
		playerReference->SetCameraStatsNav();
	}
}


void AMyAIDirector::UpdateIfIsInInner()
{
	FVector playerPos = playerReference->GetActorLocation();

	//Free Previous Array
	for (int i = 0; i <= enemiesInInner.Num() - 1; ++i)
		enemiesInInner[i]->isInInnerCircle = false;
	enemiesInInner.Empty();

	//Fill new Array
	for (int i = 0; i <= enemies.Num() - 1; ++i)
	{
		if (enemiesInInner.Num() < numberOfEnemiesInInner)
		{
			enemies[i]->isInInnerCircle = true;
			enemiesInInner.Add(enemies[i]);
		}
		else
		{
			float fartestEnemyDist{ 0 };
			ACharacter_EnemyBase* enemyToRemove{ nullptr };

			for (int j = 0; j <= enemiesInInner.Num() - 1; ++j)
			{
				if ((enemiesInInner[j]->GetActorLocation() - playerPos).Size() > fartestEnemyDist)
				{
					fartestEnemyDist = (enemiesInInner[j]->GetActorLocation() - playerPos).Size();
					enemyToRemove = enemiesInInner[j];
				}
			}

			if ((enemies[i]->GetActorLocation() - playerPos).Size() < fartestEnemyDist)
			{
				enemyToRemove->isInInnerCircle = false;
				enemiesInInner.Remove(enemyToRemove);
				enemies[i]->isInInnerCircle = true;
				enemiesInInner.Add(enemies[i]);
			}
		}
	}
}


void AMyAIDirector::SetFocusToClosestEnemy()
{
	//player will get a focus if enemies.Num > 0
	playerReference->focus = nullptr;

	FVector playerPos = playerReference->GetActorLocation();
	float distanceFromPlayer{ INFINITY };


	for (int i = 0; i <= enemies.Num() - 1; ++i)
	{
		if ((enemies[i]->GetActorLocation() - playerPos).Size() < distanceFromPlayer)
		{
			distanceFromPlayer = (enemies[i]->GetActorLocation() - playerPos).Size();
			playerReference->focus = enemies[i];
		}
	}
}

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

void AMyAIDirector::SetFocusToNextEnemy()
{
	FVector playerPos = playerReference->GetActorLocation();
	FVector vectorReference = playerReference->focus->GetActorLocation() - playerPos;
	float smallestAngle = 360;

	for (int i = 0; i <= enemies.Num() - 1; ++i)
	{
		if (enemies[i] != playerReference->focus)
		{
			FVector playerToEnemy = enemies[i]->GetActorLocation() - playerPos;
			float enemyAngle = (FVector::CrossProduct(vectorReference, playerToEnemy).Z > 0) ? acos(vectorReference.CosineAngle2D(playerToEnemy)) : 360 - acos(vectorReference.CosineAngle2D(playerToEnemy));

			if (enemyAngle < smallestAngle)
			{
				smallestAngle = enemyAngle;
				playerReference->focus = enemies[i];
			}
		}
	}
}

void AMyAIDirector::SetFocusToPreviousEnemy()
{
	FVector playerPos = playerReference->GetActorLocation();
	FVector vectorReference = playerReference->focus->GetActorLocation() - playerPos;
	float smallestAngle = 360;

	for (int i = 0; i <= enemies.Num() - 1; ++i)
	{
		if (enemies[i] != playerReference->focus)
		{
			FVector playerToEnemy = enemies[i]->GetActorLocation() - playerPos;
			float enemyAngle = (FVector::CrossProduct(vectorReference, playerToEnemy).Z < 0) ? acos(vectorReference.CosineAngle2D(playerToEnemy)) : 360 - acos(vectorReference.CosineAngle2D(playerToEnemy));

			if (enemyAngle < smallestAngle)
			{
				smallestAngle = enemyAngle;
				playerReference->focus = enemies[i];
			}
		}
	}
}
