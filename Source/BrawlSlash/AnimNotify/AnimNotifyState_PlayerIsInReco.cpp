// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_PlayerIsInReco.h"
#include "../Characters/Character_Player.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_PlayerIsInReco::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (player)
		player->isInReco = true;
}

void UAnimNotifyState_PlayerIsInReco::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (player)
		player->isInReco = false;
}