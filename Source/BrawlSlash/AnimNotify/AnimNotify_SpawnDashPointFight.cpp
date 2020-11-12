// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_SpawnDashPointFight.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Characters/Character_EnemyBase.h"
#include "../AI/MyAIDirector.h"

void UAnimNotify_SpawnDashPointFight::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());

	//if (enemy)
	//{
	//	FActorSpawnParameters spawnParams;
	//	FVector spawnPosition = enemy->GetActorLocation() + 
	//		enemy->GetActorForwardVector().RotateAngleAxis(enemy->attackCircleAngle, FVector::UpVector) * enemy->currentEnemyGroup->distanceFromSelfToDashPoint;
	//	spawnPosition.Z = enemy->currentEnemyGroup->playerReference->GetActorLocation().Z;

	//	enemy->currentEnemyGroup->dashPointInFight = enemy->GetWorld()->SpawnActor<AActor>(enemy->currentEnemyGroup->dashPointInFightClassType, spawnPosition, FRotator::ZeroRotator, spawnParams);
	//}
}