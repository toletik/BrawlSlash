// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_EnemyBase.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Character_Player.h"
#include "DrawDebugHelpers.h"

// Sets default values for this character's properties
ACharacter_EnemyBase::ACharacter_EnemyBase()
{
	attackCircle = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCircle"));
	attackCircle->SetupAttachment(RootComponent);
	attackBoxStrong = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBoxStrong"));
	attackBoxStrong->SetupAttachment(RootComponent);


	shieldFront = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldFront"));
	shieldFront->SetupAttachment(RootComponent);
	shieldBack = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldBack"));
	shieldBack->SetupAttachment(RootComponent);
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

	currentAttackCoolDown = FMath::RandRange(attackCoolDownMin, attackCoolDownMax);

	if (isShieldInFront)
		shieldFront->SetVisibility(true);
	if (isShieldInBack)
		shieldBack->SetVisibility(true);
	if(isShieldSwitchable)
		currentShieldTimeActive = shieldTimeActive;
}
// Called every frame
void ACharacter_EnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/////////////////////////////////////////////////////////////////
	//Debug
	if (isShieldInFront)
	{
		FVector selfPos = GetActorLocation();
		FVector forward = GetActorForwardVector();

		DrawDebugLine(GetWorld(), selfPos, selfPos + forward.RotateAngleAxis(angleAcceptanceForDefense, FVector::UpVector) * 200, FColor::Blue, false, 0.05, 0, 5);
		DrawDebugLine(GetWorld(), selfPos, selfPos + forward.RotateAngleAxis(-angleAcceptanceForDefense, FVector::UpVector) * 200, FColor::Blue, false, 0.05, 0, 5);
	}

	if (isShieldInBack)
	{
		FVector selfPos = GetActorLocation();
		FVector backward = -GetActorForwardVector();

		DrawDebugLine(GetWorld(), selfPos, selfPos + backward.RotateAngleAxis(angleAcceptanceForDefense, FVector::UpVector) * 200, FColor::Blue, false, 0.05, 0, 5);
		DrawDebugLine(GetWorld(), selfPos, selfPos + backward.RotateAngleAxis(-angleAcceptanceForDefense, FVector::UpVector) * 200, FColor::Blue, false, 0.05, 0, 5);
	}

	//should be done cleanly
	if (state == E_STATE::DEAD)
	{
		isShieldInFront = false;
		isShieldInBack = false;
		shieldFront->SetVisibility(false);
		shieldBack->SetVisibility(false);
	}
	/////////////////////////////////////////////////////////////////
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
		if (isInInnerCircle)
		{
			needToAttackWeak = true;
			toDoDamage = weakDamage;
		}
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


bool ACharacter_EnemyBase::ShieldCheckProtection(FVector attackerPos)
{
	if (!isShieldInFront && !isShieldInBack)
		return false;

	FVector selfToAttacker = attackerPos - GetActorLocation();
	selfToAttacker.Normalize();

	float angle = 0;

	if(isShieldInFront)
		angle = acos(FVector::DotProduct(selfToAttacker, GetActorForwardVector()) ) * 180 / PI;
	else if (isShieldInBack)
		angle = acos(FVector::DotProduct(selfToAttacker, -GetActorForwardVector()) ) * 180 / PI;

	if (angle > 180)
		angle = abs(angle - 360);

	if (angle <= angleAcceptanceForDefense)
	{
		GEngine->AddOnScreenDebugMessage(-49, 1.0f, FColor::Green, "!!!BLOCK!!!");
		return true;
	}
	
	return false;

}

void ACharacter_EnemyBase::ShieldActivate()
{
	if (isNextShieldToActivateFront)
	{
		isShieldInFront = true;
		shieldFront->SetVisibility(true);
	}
	else
	{
		isShieldInBack = true;
		shieldBack->SetVisibility(true);
	}
}
void ACharacter_EnemyBase::ShieldDeActivate()
{
	if (isShieldInFront)
	{
		isShieldInFront = false;
		shieldFront->SetVisibility(false);
		isNextShieldToActivateFront = false;
	}

	if (isShieldInBack)
	{
		isShieldInBack = false;
		shieldBack->SetVisibility(false);
		isNextShieldToActivateFront = true;
	}
}
