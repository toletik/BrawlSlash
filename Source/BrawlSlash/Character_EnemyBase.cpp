// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyBase.h"
#include "Components/SphereComponent.h"

// Called when the game starts or when spawned
void ACharacter_EnemyBase::BeginPlay()
{
	Super::BeginPlay();
	attackCircle->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_EnemyBase::AttackOverlap);
	attackCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
// Called every frame
void ACharacter_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Sets default values for this character's properties
ACharacter_EnemyBase::ACharacter_EnemyBase()
{
	attackCircle = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCircle"));
	attackCircle->SetupAttachment(RootComponent);
}

void ACharacter_EnemyBase::TakeHit(int damage)
{
	Super::TakeHit(damage);

	if (state == E_STATE::IDLE)
		state = E_STATE::HITTED_WEAK;

}

void ACharacter_EnemyBase::SetAttackState()
{
	float ratio = FMath::RandRange(0, 100);

	if (ratio <= ratioAttackWeak)
	{
		needToAttackWeak = true;
		toDoDamage = weakDamage;
	}

	else if (ratio <= ratioAttackWeak + ratioAttackStrong)
	{
		state = E_STATE::ATTACKING_STRONG;
		toDoDamage = strongDamage;
	}
}

void ACharacter_EnemyBase::BeginAttackCircle()
{
	attackCircle->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacter_EnemyBase::EndAttackCircle()
{
	attackCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
