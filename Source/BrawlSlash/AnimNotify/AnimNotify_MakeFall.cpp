// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MakeFall.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Character_Player.h"

void UAnimNotify_MakeFall::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (player)
		player->state = E_STATE::PROJECTED;
}