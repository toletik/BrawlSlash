// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_ColliderToChangeLevel.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

ALDBrick_ColliderToChangeLevel::ALDBrick_ColliderToChangeLevel()
{
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);
}

void ALDBrick_ColliderToChangeLevel::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDBrick_ColliderToChangeLevel::BoxBeginOverlap);
}

void ALDBrick_ColliderToChangeLevel::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == playerReference->GetCapsuleComponent())
	{
		UGameplayStatics::OpenLevel(GetWorld(), levelName);
	}
}