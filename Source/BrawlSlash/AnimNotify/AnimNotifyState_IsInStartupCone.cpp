// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_IsInStartupCone.h"
#include "../Characters/Character_EnemyStrong.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_IsInStartupCone::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_EnemyStrong* enemy = Cast<ACharacter_EnemyStrong>(MeshComp->GetOwner());

	if (enemy)
		enemy->isInStartupCone = true;
}

void UAnimNotifyState_IsInStartupCone::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyStrong* enemy = Cast<ACharacter_EnemyStrong>(MeshComp->GetOwner());

	if (enemy)
		enemy->isInStartupCone = false;
}