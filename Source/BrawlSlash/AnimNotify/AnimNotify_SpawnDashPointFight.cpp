// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnDashPointFight.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Characters/Character_EnemyBase.h"
#include "../AI/MyAIDirector.h"

void UAnimNotify_SpawnDashPointFight::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());

	if (enemy)
	{
	}
}