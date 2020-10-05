// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_OnAttack.h"
#include "../Character_Base.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_OnAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}


void UAnimNotify_OnAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Base* character = Cast<ACharacter_Base>(MeshComp->GetOwner());

	if (character)
		character->isAttacking = false;
}