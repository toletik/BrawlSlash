// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyStrong.h"
#include "Character_Player.h"
#include "Components/CapsuleComponent.h"

ACharacter_EnemyStrong::ACharacter_EnemyStrong()
{
	attackMeshCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttackMeshCircle"));
	attackMeshCircle->SetupAttachment(RootComponent);
	attackMeshStrong = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AttackMeshStrong"));
	attackMeshStrong->SetupAttachment(RootComponent);
}

void ACharacter_EnemyStrong::BeginPlay()
{
	Super::BeginPlay();

	attackMeshCircle->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_EnemyBase::AttackOverlap);
	attackMeshStrong->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_EnemyBase::AttackOverlap);
	attackMeshCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackMeshStrong->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_EnemyStrong::AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter_Player* playerCast = Cast<ACharacter_Player>(OtherActor);

	if (playerCast && OtherComp == Cast<UPrimitiveComponent>(playerCast->GetCapsuleComponent()))
	{
		float tempHealth = playerCast->health;
		playerCast->TakeHit(attackDamage);

		if (playerCast->health < tempHealth)
		{
			if (playerCast->state != E_STATE::DEAD
				&& ((attackMeshCircle->GetCollisionEnabled() == ECollisionEnabled::QueryOnly && attackCircleProject)
					|| (attackMeshStrong->GetCollisionEnabled() == ECollisionEnabled::QueryOnly && attackStrongProject)))
			{
				playerCast->state = E_STATE::PROJECTED;
				playerCast->PlayerStartIsProjected();
			}
			else if (playerCast->state == E_STATE::IDLE)
				playerCast->state = E_STATE::HITTED_STRONG;
			playerCast->PlayerStartHittedStrong();
		}
	}
}

void ACharacter_EnemyStrong::SetAttackState()
{
	state = E_STATE::ATTACKING_STRONG;
	EnemyStartAttackStrong();

	if (isInInnerCircle)
		currentAttackCoolDown = FMath::RandRange(attackCoolDownInnerMin, attackCoolDownInnerMax);
	else
		currentAttackCoolDown = FMath::RandRange(attackCoolDownBackMin, attackCoolDownBackMax);
}

void ACharacter_EnemyStrong::BeginAttackCircle()
{
	attackMeshCircle->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacter_EnemyStrong::EndAttackCircle()
{
	attackMeshCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_EnemyStrong::BeginAttackStrong()
{
	attackMeshStrong->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacter_EnemyStrong::EndAttackStrong()
{
	attackMeshStrong->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
