// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_CanCombo.h"
#include "../Character_Player.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_CanCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (character)
		character->canCombo = true;
}

void UAnimNotifyState_CanCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (character)
		character->canCombo = false;
}