// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackCircle.h"
#include "../Character_EnemyBase.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_AttackCircle::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_EnemyBase* character = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());

	if (character)
		character->BeginAttackCircle();
}

void UAnimNotifyState_AttackCircle::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyBase* character = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());

	if (character)
		character->EndAttackCircle();
}
