// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackIfNeedTo.h"
#include "../Characters/Character_Player.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"

void UAnimNotify_AttackIfNeedTo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (player && player->needToAttack)
	{
		player->needToAttack = false;
		player->SetNextAttackCombo();
	}
}