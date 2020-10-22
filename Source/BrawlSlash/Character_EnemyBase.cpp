// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyBase.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Character_Player.h"

// Sets default values for this character's properties
ACharacter_EnemyBase::ACharacter_EnemyBase()
{
	attackCircle = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCircle"));
	attackCircle->SetupAttachment(RootComponent);
	attackBoxStrong = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBoxStrong"));
	attackBoxStrong->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACharacter_EnemyBase::BeginPlay()
{
	Super::BeginPlay();

	attackBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_EnemyBase::AttackOverlap);
	attackCircle->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_EnemyBase::AttackOverlap);
	attackCircle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	attackBoxStrong->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_EnemyBase::AttackOverlap);
	attackBoxStrong->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
// Called every frame
void ACharacter_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacter_EnemyBase::AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter_Player* playerCast = Cast<ACharacter_Player>(OtherActor);

	if (playerCast)
	{
		playerCast->TakeHit(toDoDamage, state);
		toDoDamage = 0;
	}
}

void ACharacter_EnemyBase::TakeHit(int damage, E_STATE attackerState)
{
	Super::TakeHit(damage, attackerState);

	if (state != E_STATE::IDLE)
		HitOther();

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

void ACharacter_EnemyBase::BeginAttackStrong()
{
	attackBoxStrong->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacter_EnemyBase::EndAttackStrong()
{
	attackBoxStrong->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}