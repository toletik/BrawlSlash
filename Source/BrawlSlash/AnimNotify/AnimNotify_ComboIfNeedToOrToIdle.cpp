// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ComboIfNeedToOrToIdle.h"
#include "../Characters/Character_Player.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_ComboIfNeedToOrToIdle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (player)
	{
		if (player->needToAttack)
		{
			player->needToAttack = false;
			player->SetNextAttackCombo();
		}
		else if (player->state != E_STATE::DEAD && player->state != CINEMATIC)
			player->state = E_STATE::IDLE;
	}
}
