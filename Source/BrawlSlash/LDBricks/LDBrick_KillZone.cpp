// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_KillZone.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "../MyGameInstance.h"

ALDBrick_KillZone::ALDBrick_KillZone()
{
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);
}

void ALDBrick_KillZone::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	gameInstanceReference = Cast<UMyGameInstance>(GetGameInstance());

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDBrick_KillZone::BoxBeginOverlap);
}

void ALDBrick_KillZone::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == playerReference->GetCapsuleComponent())
	{
		gameInstanceReference->numberOfPlayerShieldPoints = playerReference->currentShieldPoint;
		UGameplayStatics::OpenLevel(GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(GetWorld())));
	}
}