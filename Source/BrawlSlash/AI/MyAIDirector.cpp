// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIDirector.h"

#include "MyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "../Characters/Character_EnemyStrong.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

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

	for (int i = 0; i < enemies.Num(); i++)
		enemies[i]->currentEnemyGroup = this;
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
		UpdateIfIsRespectingAngularDist();


}

void AMyAIDirector::UpdateIfNeedToStartFight()
{
	if (playerReference->isInSafeZone)
		return;

	for (int i = 0; i < enemies.Num(); ++i)
	{
		if ((enemies[i]->GetActorLocation() - playerReference->GetActorLocation()).Size() < radiusBackCircle)
		{
			StartFight();
			return;
		}
	}
}

void AMyAIDirector::StartFight()
{
	isInFight = true;

	for (int j = 0; j < enemies.Num(); ++j)
		enemies[j]->EnemyStartFight();

	playerReference->currentEnemyGroup = this;

	UpdateIfIsInInner();
	SetFocusToClosestEnemy();

	playerReference->SetCameraStatsFight(rotationForTheFight, minDistanceToAdoptForCamera, maxDistanceToAdoptForCamera);
	playerReference->debugInnerCircleRadius = radiusInnerCircle;
	playerReference->debugBackCircleRadius = radiusBackCircle;
	playerReference->PlayerStartFight();
	playerReference->GetCharacterMovement()->MaxWalkSpeed = playerReference->walkSpeedFight;
}

void AMyAIDirector::SetEndToFight()
{
	isInFight = false;

	playerReference->currentEnemyGroup = nullptr;
	playerReference->focus = nullptr;
	playerReference->SetCameraStatsNav();
	playerReference->SetFocusToClosestFocus();
	playerReference->PlayerEndFight();
	playerReference->GetCharacterMovement()->MaxWalkSpeed = playerReference->walkSpeedNav;
	playerReference->health = playerReference->maxHealth;
}

void AMyAIDirector::RemoveEnemy(ACharacter_EnemyBase* enemyToRemove)
{
	if (playerReference->focus == enemyToRemove)
		SetFocusToClosestEnemy();

	if (enemies.Num() == 1)
	{
		enemyToRemove->LastEnemyInGroup();
		AIDirectorAllEnemiesDead();
		SetEndToFight();
	}

	else
		enemyToRemove->EnemyDeath();

	enemies.Remove(enemyToRemove);
	
	if (vipEnemies.Num() == 1 && vipEnemies.Contains(enemyToRemove) && sequenceToPlay)
	{
		ALevelSequenceActor* temp;
		playerReference->currentSequence = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequenceToPlay, FMovieSceneSequencePlaybackSettings(), temp);
		playerReference->currentSequence->Play();
		playerReference->isSequenceSkippable = isSequenceSkippable;
		if (stopPlayerMovementOnSequence)
			playerReference->state = CINEMATIC;
	}

	vipEnemies.Remove(enemyToRemove);

	UpdateIfIsInInner();
}

void AMyAIDirector::UpdateIfIsInInner()
{
	FVector playerPos = playerReference->GetActorLocation();

	//Free Previous Array
	for (int i = 0; i < enemiesInInner.Num(); ++i)
		enemiesInInner[i]->isInInnerCircle = false;
	enemiesInInner.Empty();
	hasAStrongEnemyInInner = false;

	//Fill new Array
	for (int i = 0; i < enemies.Num(); ++i)
	{
		ACharacter_EnemyStrong* strongEnemy = Cast<ACharacter_EnemyStrong>(enemies[i]);
		if (strongEnemy && hasAStrongEnemyInInner == true)
			continue;
		else if (enemiesInInner.Num() < numberOfEnemiesToHaveInInner)
		{
			enemies[i]->isInInnerCircle = true;
			enemiesInInner.Add(enemies[i]);

			if (strongEnemy)
				hasAStrongEnemyInInner = true;
		}
		else
		{
			float fartestEnemyDist{ 0 };
			ACharacter_EnemyBase* enemyToRemove{ nullptr };

			for (int j = 0; j < enemiesInInner.Num(); ++j)
			{
				float distanceFromPlayer = (enemiesInInner[j]->GetActorLocation() - playerPos).Size();
				if (distanceFromPlayer > fartestEnemyDist)
				{
					fartestEnemyDist = distanceFromPlayer;
					enemyToRemove = enemiesInInner[j];
				}
			}

			if ((enemies[i]->GetActorLocation() - playerPos).Size() < fartestEnemyDist)
			{
				enemyToRemove->isInInnerCircle = false;
				enemiesInInner.Remove(enemyToRemove);
				GetWorldTimerManager().ClearTimer(enemyToRemove->timerHandler);
				enemyToRemove->LookAtPlayer();

				enemies[i]->isInInnerCircle = true;
				enemiesInInner.Add(enemies[i]);
			}
			else
			{
				GetWorldTimerManager().ClearTimer(enemies[i]->timerHandler);
				enemies[i]->LookAtPlayer();
			}
		}
	}
}

void AMyAIDirector::UpdateIfIsRespectingAngularDist()
{
	FVector playerPos = playerReference->GetActorLocation();

	for (int i = 0; i < enemies.Num(); ++i)
	{
		FVector vectorReference = enemies[i]->GetActorLocation() - playerPos;
		vectorReference.Normalize();
		enemies[i]->isRespectingAngularDist = true;

		for (int j = 0; j < enemies.Num(); ++j)
		{
			if (enemies[i] != enemies[j])
			{
				FVector playerToEnemy = enemies[j]->GetActorLocation() - playerPos;
				playerToEnemy.Normalize();
				float enemyAngle = (FVector::CrossProduct(vectorReference, playerToEnemy).Z > 0) ? FMath::RadiansToDegrees( acos(vectorReference.CosineAngle2D(playerToEnemy)) ) : 360 - FMath::RadiansToDegrees( acos(vectorReference.CosineAngle2D(playerToEnemy)) );

				if (enemyAngle < angularDisToRespect)
				{
					enemies[i]->isRespectingAngularDist = false;
				}
			}
		}
	}
}



bool AMyAIDirector::IsAllEnemiesInCameraSight(APlayerController* controller)
{

	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	float percentageX {ViewportSize.X / 100};
	float percentageY {ViewportSize.Y / 100};

	for (int i = 0; i < enemies.Num(); ++i)
	{
		FVector2D enemyPosOnScreen;
		UGameplayStatics::ProjectWorldToScreen(controller, enemies[i]->GetActorLocation(), enemyPosOnScreen);

		if (offsetFromLeft * percentageX < enemyPosOnScreen.X && enemyPosOnScreen.X < ViewportSize.X - offsetFromRight * percentageX  &&
			offsetFromBottom * percentageY < enemyPosOnScreen.Y && enemyPosOnScreen.Y < ViewportSize.Y - offsetFromTop * percentageY)
		{
			//do Something
		}
		else
			return false;
	}

	return true;
}

void AMyAIDirector::SetFocusToClosestEnemy()
{
	AActor* previousFocus = playerReference->focus;
	//player will get a focus if enemies.Num > 0
	playerReference->focus = nullptr;

	FVector playerPos = playerReference->GetActorLocation();
	float distanceFromPlayer{ INFINITY };


	for (int i = 0; i < enemies.Num(); ++i)
	{
		if (enemies[i] != previousFocus && (enemies[i]->GetActorLocation() - playerPos).Size() < distanceFromPlayer)
		{
			distanceFromPlayer = (enemies[i]->GetActorLocation() - playerPos).Size();
			playerReference->focus = enemies[i];
		}
	}
}

void AMyAIDirector::SetFocusToNextEnemy()
{
	FVector cameraPos = playerReference->followCamera->GetComponentLocation();
	FVector vectorReference = playerReference->focus->GetActorLocation() - cameraPos;
	float smallestAngle = 360;

	if (dashPointInFight && dashPointInFight != playerReference->focus)
	{
		FVector cameraToDashPoint = dashPointInFight->GetActorLocation() - cameraPos;

		float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToDashPoint).Z > 0) ? acos(vectorReference.CosineAngle2D(cameraToDashPoint)) : 360 - acos(vectorReference.CosineAngle2D(cameraToDashPoint));
		smallestAngle = enemyAngle;
		playerReference->focus = dashPointInFight;
	}

	for (int i = 0; i < enemies.Num(); ++i)
	{
		if (enemies[i] != playerReference->focus)
		{
			FVector cameraToEnemy = enemies[i]->GetActorLocation() - cameraPos;
			float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToEnemy).Z > 0) ? acos(vectorReference.CosineAngle2D(cameraToEnemy)) : 360 - acos(vectorReference.CosineAngle2D(cameraToEnemy));

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
	FVector cameraPos = playerReference->followCamera->GetComponentLocation();
	FVector vectorReference = playerReference->focus->GetActorLocation() - cameraPos;
	float smallestAngle = 360;

	if (dashPointInFight && dashPointInFight != playerReference->focus)
	{
		FVector cameraToDashPoint = dashPointInFight->GetActorLocation() - cameraPos;

		float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToDashPoint).Z < 0) ? acos(vectorReference.CosineAngle2D(cameraToDashPoint)) : 360 - acos(vectorReference.CosineAngle2D(cameraToDashPoint));
		smallestAngle = enemyAngle;
		playerReference->focus = dashPointInFight;
	}

	for (int i = 0; i < enemies.Num(); ++i)
	{
		if (enemies[i] != playerReference->focus)
		{
			FVector cameraToEnemy = enemies[i]->GetActorLocation() - cameraPos;
			float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToEnemy).Z < 0) ? acos(vectorReference.CosineAngle2D(cameraToEnemy)) : 360 - acos(vectorReference.CosineAngle2D(cameraToEnemy));

			if (enemyAngle < smallestAngle)
			{
				smallestAngle = enemyAngle;
				playerReference->focus = enemies[i];
			}
		}
	}
}

void AMyAIDirector::SetDebugFocusToNextEnemy()
{
	FVector cameraPos = playerReference->followCamera->GetComponentLocation();
	FVector vectorReference = playerReference->focus->GetActorLocation() - cameraPos;
	float smallestAngle = 360;

	if (dashPointInFight && dashPointInFight != playerReference->focus)
	{
		FVector cameraToDashPoint = dashPointInFight->GetActorLocation() - cameraPos;

		float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToDashPoint).Z > 0) ? acos(vectorReference.CosineAngle2D(cameraToDashPoint)) : 360 - acos(vectorReference.CosineAngle2D(cameraToDashPoint));
		smallestAngle = enemyAngle;
		playerReference->debugNextFocus = dashPointInFight;
	}

	for (int i = 0; i < enemies.Num(); ++i)
	{
		if (enemies[i] != playerReference->focus)
		{
			FVector cameraToEnemy = enemies[i]->GetActorLocation() - cameraPos;
			float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToEnemy).Z > 0) ? acos(vectorReference.CosineAngle2D(cameraToEnemy)) : 360 - acos(vectorReference.CosineAngle2D(cameraToEnemy));

			if (enemyAngle < smallestAngle)
			{
				smallestAngle = enemyAngle;
				playerReference->debugNextFocus = enemies[i];
			}
		}
	}
}
void AMyAIDirector::SetDebugFocusToPreviousEnemy()
{
	FVector cameraPos = playerReference->followCamera->GetComponentLocation();
	FVector vectorReference = playerReference->focus->GetActorLocation() - cameraPos;
	float smallestAngle = 360;

	if (dashPointInFight && dashPointInFight != playerReference->focus)
	{
		FVector cameraToDashPoint = dashPointInFight->GetActorLocation() - cameraPos;

		float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToDashPoint).Z < 0) ? acos(vectorReference.CosineAngle2D(cameraToDashPoint)) : 360 - acos(vectorReference.CosineAngle2D(cameraToDashPoint));
		smallestAngle = enemyAngle;
		playerReference->debugPreviousFocus = dashPointInFight;
	}

	for (int i = 0; i < enemies.Num(); ++i)
	{
		if (enemies[i] != playerReference->focus)
		{
			FVector cameraToEnemy = enemies[i]->GetActorLocation() - cameraPos;
			float enemyAngle = (FVector::CrossProduct(vectorReference, cameraToEnemy).Z < 0) ? acos(vectorReference.CosineAngle2D(cameraToEnemy)) : 360 - acos(vectorReference.CosineAngle2D(cameraToEnemy));

			if (enemyAngle < smallestAngle)
			{
				smallestAngle = enemyAngle;
				playerReference->debugPreviousFocus = enemies[i];
			}
		}
	}
}
