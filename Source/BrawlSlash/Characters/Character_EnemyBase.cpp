// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyBase.h"
#include "Character_Player.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "../AI/MyAIDirector.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACharacter_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void ACharacter_EnemyBase::TakeHit(int damage)
{
	Super::TakeHit(damage);

	if (health > 0)
	{
		//can use a : UGameplayStatics::GetPlayerCharacter(GetWorld(), 0); but access a ptr cost less
		FVector direction = GetActorLocation() - currentEnemyGroup->playerReference->GetActorLocation();
		direction.Normalize();
		LaunchCharacter(direction * pushForceAfterBeingHit, true, true);

		EnemyHitted();

		if (state == E_STATE::IDLE)
			state = E_STATE::HITTED_WEAK;
	}
	else
		currentEnemyGroup->RemoveEnemy(this);
}

void ACharacter_EnemyBase::LookAtPlayer()
{
	beingDashedBack = false;
	rotateDashedBack = true;
}

bool ACharacter_EnemyBase::IsPlayerInFront(FVector playerPos)
{
	return FVector::DotProduct(GetActorLocation() - playerPos, GetActorForwardVector()) < 0;
}