// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyWeakWithShield.h"
#include "Component_EnemyShield.h"

ACharacter_EnemyWeakWithShield::ACharacter_EnemyWeakWithShield()
{
	shieldComponent = CreateDefaultSubobject<UComponent_EnemyShield>(TEXT("ShieldComponent"));
	AddOwnedComponent(shieldComponent);
	shieldComponent->AttachToOwner();
}

void ACharacter_EnemyWeakWithShield::TakeHit(int damage)
{
	Super::TakeHit(damage);

	if (health < 0)
		shieldComponent->OnOwnerDeath();
}
