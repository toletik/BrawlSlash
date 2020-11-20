// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_EnemyWeakAttack.h"
#include "../Characters/Character_EnemyWeak.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotifyState_EnemyWeakAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_EnemyWeak* enemyWeak = Cast<ACharacter_EnemyWeak>(MeshComp->GetOwner());

	if (enemyWeak)
		enemyWeak->BeginAttack();
}

void UAnimNotifyState_EnemyWeakAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyWeak* enemyWeak = Cast<ACharacter_EnemyWeak>(MeshComp->GetOwner());
	
	if (enemyWeak)
		enemyWeak->EndAttack();
}
