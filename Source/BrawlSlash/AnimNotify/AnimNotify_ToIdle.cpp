// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_ToIdle.h"
#include "../Character_Base.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"

void UAnimNotify_ToIdle::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Base* character = Cast<ACharacter_Base>(MeshComp->GetOwner());

	if (character)
		character->state = E_STATE::IDLE;
}