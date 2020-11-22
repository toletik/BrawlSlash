// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnDashPointFight.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Characters/Character_EnemyStrongWithShield.h"
#include "../AI/MyAIDirector.h"

void UAnimNotify_SpawnDashPointFight::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_EnemyStrongWithShield* enemy = Cast<ACharacter_EnemyStrongWithShield>(MeshComp->GetOwner());

	if (enemy)
	{
		FActorSpawnParameters spawnParams;
		FVector spawnPosition = enemy->GetActorLocation() + enemy->GetActorForwardVector().RotateAngleAxis(enemy->attackCircleAngle, FVector::UpVector) * enemy->distanceFromSelfToDashPoint;
		spawnPosition.Z = enemy->currentEnemyGroup->playerReference->GetActorLocation().Z;

		enemy->currentEnemyGroup->dashPointInFight = enemy->GetWorld()->SpawnActor<AActor>(enemy->dashPointInFightClassType, spawnPosition, FRotator::ZeroRotator, spawnParams);
	}
}