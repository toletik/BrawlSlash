// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_SafeZone.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../AI/MyAIDirector.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

ALDBrick_SafeZone::ALDBrick_SafeZone()
{
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);
}

void ALDBrick_SafeZone::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDBrick_SafeZone::BoxBeginOverlap);
	boxCollider->OnComponentEndOverlap.AddDynamic(this, &ALDBrick_SafeZone::BoxEndOverlap);
}

void ALDBrick_SafeZone::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == playerReference->GetCapsuleComponent())
	{
		playerReference->isInSafeZone = true;

		if (playerReference->currentEnemyGroup)
			playerReference->currentEnemyGroup->SetEndToFight();
	}
}

void ALDBrick_SafeZone::BoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp == playerReference->GetCapsuleComponent())
		playerReference->isInSafeZone = false;
}


