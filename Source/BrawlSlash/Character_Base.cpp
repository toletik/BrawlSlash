// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = maxHealth;
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacter_Base::Attack()
{
	isAttacking = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Attack");
}
