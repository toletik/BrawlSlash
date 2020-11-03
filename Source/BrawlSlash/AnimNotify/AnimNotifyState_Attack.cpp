// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_Attack.h"
#include "../Characters/Character_Base.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_Base* character = Cast<ACharacter_Base>(MeshComp->GetOwner());

	if (character)
		character->BeginAttack();
}

void UAnimNotifyState_Attack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Base* character = Cast<ACharacter_Base>(MeshComp->GetOwner());

	if (character)
		character->EndAttack();
}