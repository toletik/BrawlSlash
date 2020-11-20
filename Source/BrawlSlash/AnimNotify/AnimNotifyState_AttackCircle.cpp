// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_AttackCircle.h"
#include "../Characters/Character_EnemyStrong.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_AttackCircle::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_EnemyStrong* enemyStrong = Cast<ACharacter_EnemyStrong>(MeshComp->GetOwner());

	if (enemyStrong)
		enemyStrong->BeginAttackCircle();
}

void UAnimNotifyState_AttackCircle::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyStrong* enemyStrong = Cast<ACharacter_EnemyStrong>(MeshComp->GetOwner());

	if (enemyStrong)
		enemyStrong->EndAttackCircle();
}
