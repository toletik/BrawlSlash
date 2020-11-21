// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_CheckPoint.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "../MyGameInstance.h"

ALDBrick_CheckPoint::ALDBrick_CheckPoint()
{
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);
}

void ALDBrick_CheckPoint::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	gameInstanceReference = Cast<UMyGameInstance>(GetGameInstance());

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDBrick_CheckPoint::BoxBeginOverlap);
}

void ALDBrick_CheckPoint::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == playerReference->GetCapsuleComponent())
	{
		gameInstanceReference->lastPlayerPos = playerReference->GetActorLocation();
		gameInstanceReference->lastPlayerRot = rotationForRespawn;
		gameInstanceReference->hasRestartLevel = true;
	}
}
