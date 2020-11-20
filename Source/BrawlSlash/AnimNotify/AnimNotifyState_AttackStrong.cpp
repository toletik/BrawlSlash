// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackStrong.h"
#include "../Characters/Character_EnemyStrong.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_AttackStrong::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_EnemyStrong* enemyStrong = Cast<ACharacter_EnemyStrong>(MeshComp->GetOwner());

	if (enemyStrong)
		enemyStrong->BeginAttackStrong();
}

void UAnimNotifyState_AttackStrong::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyStrong* enemyStrong = Cast<ACharacter_EnemyStrong>(MeshComp->GetOwner());

	if (enemyStrong)
		enemyStrong->EndAttackStrong();
}