// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_PlayerAttack.h"
#include "../Characters/Character_Player.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_PlayerAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (character)
		character->BeginAttack();
}

void UAnimNotifyState_PlayerAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
		ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());
	
		if (character)
			character->EndAttack();
}
