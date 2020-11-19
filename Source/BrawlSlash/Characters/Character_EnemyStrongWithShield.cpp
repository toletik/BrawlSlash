// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyStrongWithShield.h"
#include "Component_EnemyShield.h"

ACharacter_EnemyStrongWithShield::ACharacter_EnemyStrongWithShield()
{
	shieldComponent = CreateDefaultSubobject<UComponent_EnemyShield>(TEXT("ShieldComponent"));
	AddOwnedComponent(shieldComponent);
	shieldComponent->AttachToOwner();
}

void ACharacter_EnemyStrongWithShield::TakeHit(int damage)
{
	Super::TakeHit(damage);

	if (health < 0)
		shieldComponent->OnOwnerDeath();
}
