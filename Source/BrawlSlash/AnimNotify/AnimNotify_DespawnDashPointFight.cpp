// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DespawnDashPointFight.h"

#include "Components/SkeletalMeshComponent.h"
#include "../Characters/Character_Player.h"
#include "../Characters/Character_EnemyStrong.h"
#include "../AI/MyAIDirector.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotify_DespawnDashPointFight::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyStrong* enemyStrong = Cast<ACharacter_EnemyStrong>(MeshComp->GetOwner());

	if (enemyStrong && enemyStrong->currentEnemyGroup->dashPointInFight)
	{
		ACharacter_Player* player = enemyStrong->currentEnemyGroup->playerReference;

		if (player->focus == enemyStrong->currentEnemyGroup->dashPointInFight)
		{
			enemyStrong->currentEnemyGroup->SetFocusToClosestEnemy();

			if (player->state == DASHING_HIT)
			{
				player->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
				player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
				player->state = IDLE;
			}
		}
		
		enemyStrong->currentEnemyGroup->dashPointInFight->Destroy();
		enemyStrong->currentEnemyGroup->dashPointInFight = nullptr;
	}
}