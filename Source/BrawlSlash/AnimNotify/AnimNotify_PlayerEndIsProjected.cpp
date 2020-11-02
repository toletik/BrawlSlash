// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayerEndIsProjected.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Characters/Character_Player.h"

void UAnimNotify_PlayerEndIsProjected::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (player)
		player->PlayerEndIsProjected();
}