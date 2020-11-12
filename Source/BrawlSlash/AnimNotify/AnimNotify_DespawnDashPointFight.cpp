// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DespawnDashPointFight.h"

#include "Components/SkeletalMeshComponent.h"
#include "../Characters/Character_Player.h"
#include "../Characters/Character_EnemyBase.h"
#include "../AI/MyAIDirector.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAnimNotify_DespawnDashPointFight::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());

	//if (enemy && enemy->currentEnemyGroup->dashPointInFight)
	//{
	//	ACharacter_Player* player = enemy->currentEnemyGroup->playerReference;

	//	if (player->focus == enemy->currentEnemyGroup->dashPointInFight)
	//	{
	//		enemy->currentEnemyGroup->SetFocusToClosestEnemy();

	//		if (player->state == DASHING_HIT)
	//		{
	//			player->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	//			player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	//			player->state = IDLE;
	//		}
	//	}
	//	
	//	enemy->currentEnemyGroup->dashPointInFight->Destroy();
	//}
}