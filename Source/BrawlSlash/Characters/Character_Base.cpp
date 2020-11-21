// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/BoxComponent.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACharacter_Base::TakeHit(int damage)
{
	health -= damage;

	if (health <= 0)
	{
		SetActorEnableCollision(false);
		state = E_STATE::DEAD;
	}
}