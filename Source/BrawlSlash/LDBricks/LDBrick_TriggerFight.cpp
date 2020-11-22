// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_TriggerFight.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "../AI/MyAIDirector.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ALDBrick_TriggerFight::ALDBrick_TriggerFight()
{
    boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
    boxCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALDBrick_TriggerFight::BeginPlay()
{
    Super::BeginPlay();
    playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDBrick_TriggerFight::BoxBeginOverlap);
}

void ALDBrick_TriggerFight::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherComp == playerReference->GetCapsuleComponent() && !playerReference->isInSafeZone && enemyGroupToTrigger && !enemyGroupToTrigger->IsPendingKill())
        enemyGroupToTrigger->StartFight();
}