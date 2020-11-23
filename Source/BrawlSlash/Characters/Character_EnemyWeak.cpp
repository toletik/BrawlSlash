// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyWeak.h"
#include "Character_Player.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

ACharacter_EnemyWeak::ACharacter_EnemyWeak()
{
	attackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	attackBox->SetupAttachment(RootComponent);
}

void ACharacter_EnemyWeak::BeginPlay()
{
	Super::BeginPlay();

	attackBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_EnemyBase::AttackOverlap);
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_EnemyWeak::AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter_Player* playerCast = Cast<ACharacter_Player>(OtherActor);

	if (playerCast && OtherComp == Cast<UPrimitiveComponent>(playerCast->GetCapsuleComponent()))
	{
		float tempHealth = playerCast->health;
		playerCast->TakeHit(attackDamage);

		if (playerCast->health < tempHealth)
		{
			if (playerCast->state == E_STATE::IDLE)
				playerCast->state = E_STATE::HITTED_WEAK;
			playerCast->PlayerStartHittedWeak();
		}
	}
}

void ACharacter_EnemyWeak::SetAttackState()
{
	if (isInInnerCircle)
	{
		needToAttackWeak = true;
		EnemyStartNeedAttackWeak();
	}

	currentAttackCoolDown = FMath::RandRange(attackCoolDownMin, attackCoolDownMax);
}

void ACharacter_EnemyWeak::BeginAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacter_EnemyWeak::EndAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}