// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"

#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInterface.h"

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

	USkeletalMeshComponent* mesh = GetMesh();
	UMaterialInterface* mat = mesh->GetMaterial(0);
	matDynamic = mesh->CreateDynamicMaterialInstance(0, mat);

}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacter_Base::Attack()
{
	state = E_STATE::ATTACKING;
}
