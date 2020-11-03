// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_CanCancelCombo.h"
#include "../Characters/Character_Player.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_CanCancelCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (character)
		character->canCancelCombo = true;
}

void UAnimNotifyState_CanCancelCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (character)
		character->canCancelCombo = false;
}
