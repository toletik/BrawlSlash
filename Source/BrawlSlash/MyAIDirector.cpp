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

	if (enemies.Num() == 0 && enemiesInInner.Num() == 0)
		return;

	GEngine->AddOnScreenDebugMessage(-10, 0.5f, FColor::Blue, FString::FromInt(enemiesInInner.Num()) );

	for (int i = 0; i < enemiesInInner.Num() - 1 ; ++i)
		GEngine->AddOnScreenDebugMessage(-i - 7 , 0.5f, FColor::Purple, enemiesInInner[i]->GetName());
		

	UpdateDead();

	UpdatePosition();

	UpdateAttack(DeltaTime);

}


void AMyAIDirector::UpdateDead()
{
	for (int i = enemiesInInner.Num() - 1; i >= 0; --i)
		if (enemiesInInner[i]->state == E_STATE::DEAD)
			enemiesInInner.RemoveAt(i);


	for (int i = enemies.Num() - 1; i >= 0; --i)
		if (enemies[i]->state == E_STATE::DEAD)
			enemies.RemoveAt(i);


}

void AMyAIDirector::UpdatePosition()
{
	if (enemiesInInner.Num() < numberOfEnemiesInInner)
		for (int i = enemies.Num() - 1; enemiesInInner.Num() != numberOfEnemiesInInner && i >= 0; --i)
			if (!enemies[i]->isInInnerCircle)
			{
				enemies[i]->isInInnerCircle = true;
				enemies[i]->SetIfNeedToGlow(true);

				enemiesInInner.Add(enemies[i]);

				enemies.RemoveAt(i);
			}
	
}


void AMyAIDirector::UpdateAttack(float DeltaTime)
{
	remainingTimeForNextAttack -= DeltaTime;

	if (remainingTimeForNextAttack <= 0)
	{
		remainingTimeForNextAttack = timeBetweenAttacks;

		ACharacter_EnemyBase* randomEnemy = enemiesInInner[FMath::RandRange(0, enemiesInInner.Num() - 1)];
		
		randomEnemy->needToAttack = true;
	}
}

