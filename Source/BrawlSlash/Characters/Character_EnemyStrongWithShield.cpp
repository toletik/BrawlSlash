// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyStrongWithShield.h"
#include "Component_EnemyShield.h"
#include "Components/StaticMeshComponent.h"
#include "../AI/MyAIDirector.h"

ACharacter_EnemyStrongWithShield::ACharacter_EnemyStrongWithShield()
{
	shieldComponent = CreateDefaultSubobject<UComponent_EnemyShield>(TEXT("ShieldComponent"));
	AddOwnedComponent(shieldComponent);

	meshShieldFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldFront"));
	meshShieldBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldBack"));
	meshShieldFront->SetupAttachment(RootComponent);
	meshShieldBack->SetupAttachment(RootComponent);

	shieldComponent->shieldFront = meshShieldFront;
	shieldComponent->shieldBack = meshShieldBack;
}

void ACharacter_EnemyStrongWithShield::TakeHit(int damage)
{
	Super::TakeHit(damage);

	if (health < 0)
	{
		if (currentEnemyGroup->dashPointInFight)
		{
			currentEnemyGroup->dashPointInFight->Destroy();
			currentEnemyGroup->dashPointInFight = nullptr;
		}
	}
}