// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_OnCounter.h"
#include "../Character_Player.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_OnCounter::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

}


void UAnimNotify_OnCounter::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());

	if (character)
		character->isCountering = false;
}