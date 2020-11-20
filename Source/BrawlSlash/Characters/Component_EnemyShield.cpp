// Fill out your copyright notice in the Description page of Project Settings.


#include "Component_EnemyShield.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UComponent_EnemyShield::UComponent_EnemyShield()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UComponent_EnemyShield::BeginPlay()
{
	Super::BeginPlay();

	if (isShieldInFront)
		shieldFront->SetVisibility(true);
	if (isShieldInBack)
		shieldBack->SetVisibility(true);
	if (isShieldSwitchable)
		currentShieldTimeActive = shieldTimeActive;
}


// Called every frame
void UComponent_EnemyShield::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UComponent_EnemyShield::OnOwnerDeath()
{
	shieldFront->SetVisibility(false);
	shieldBack->SetVisibility(false);
}

bool UComponent_EnemyShield::ShieldCheckProtection(FVector attackerPos)
{
	if (!isShieldInFront && !isShieldInBack)
		return false;

	AActor* owner = GetOwner();
	FVector selfToAttacker = attackerPos - owner->GetActorLocation();
	selfToAttacker.Normalize();


	if (isShieldInFront)
	{
		float angle = acos(FVector::DotProduct(selfToAttacker, owner->GetActorForwardVector())) * 180 / PI;

		if (angle > 180)
			angle = 360 - angle;

		if (angle <= angleAcceptanceForDefense)
		{
			ShieldFrontHitted.Broadcast();
			return true;
		}
	}

	if (isShieldInBack)
	{
		float angle = acos(FVector::DotProduct(selfToAttacker, -owner->GetActorForwardVector())) * 180 / PI;

		if (angle > 180)
			angle = 360 - angle;

		if (angle <= angleAcceptanceForDefense)
		{
			ShieldBackHitted.Broadcast();
			return true;
		}
	}

	return false;
}

void UComponent_EnemyShield::ShieldActivate()
{
	if (isNextShieldToCycleFront)
	{
		isShieldInFront = true;
		EnemyShieldFrontStartActive.Broadcast();
	}
	else
	{
		isShieldInBack = true;
		EnemyShieldBackStartActive.Broadcast();
	}

	isNextShieldToCycleFront = !isNextShieldToCycleFront;
}
void UComponent_EnemyShield::ShieldDeActivate()
{
	if (isNextShieldToCycleFront)
	{
		isShieldInFront = false;
		shieldFront->SetVisibility(false);
		EnemyShieldFrontStartDeactive.Broadcast();
	}
	else
	{
		isShieldInBack = false;
		shieldBack->SetVisibility(false);
		EnemyShieldBackStartDeactive.Broadcast();
	}
}

void UComponent_EnemyShield::ShieldToActivate()
{
	if (isNextShieldToCycleFront)
	{
		EnemyShieldFrontStartToActive.Broadcast();
		shieldFront->SetVisibility(true);
	}
	else
	{
		EnemyShieldBackStartToActive.Broadcast();
		shieldBack->SetVisibility(true);
	}
}

void UComponent_EnemyShield::ShieldToDeActivate()
{
	if (isNextShieldToCycleFront)
		EnemyShieldFrontStartToDeactive.Broadcast();
	else
		EnemyShieldBackStartToDeactive.Broadcast();
}