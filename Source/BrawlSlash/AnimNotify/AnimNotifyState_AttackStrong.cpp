// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackStrong.h"
#include "../Characters/Character_EnemyBase.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_AttackStrong::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());

	if (enemy)
		enemy->BeginAttackStrong();
}

void UAnimNotifyState_AttackStrong::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());

	if (enemy)
		enemy->EndAttackStrong();
}