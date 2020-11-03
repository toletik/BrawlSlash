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


	attackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	attackBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	health = maxHealth;

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_Base::BeginAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacter_Base::EndAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_Base::TakeHit(int damage, E_STATE attackerState)
{
	health -= damage;

	if (health <= 0)
	{
		SetActorEnableCollision(false);
		state = E_STATE::DEAD;
	}
}