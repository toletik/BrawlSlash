// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_TriggerSequenceCollider.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

ALDBrick_TriggerSequenceCollider::ALDBrick_TriggerSequenceCollider()
{
	boxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	boxCollider->SetupAttachment(RootComponent);
}

void ALDBrick_TriggerSequenceCollider::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	boxCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDBrick_TriggerSequenceCollider::BoxBeginOverlap);
}

void ALDBrick_TriggerSequenceCollider::BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == playerReference->GetCapsuleComponent())
	{
		ALevelSequenceActor* temp;
		playerReference->currentSequence = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequenceToPlay, FMovieSceneSequencePlaybackSettings(), temp);
		playerReference->currentSequence->Play();
		playerReference->isSequenceSkippable = isSequenceSkippable;
		if (stopPlayerMovementOnSequence)
			playerReference->state = CINEMATIC;
		Destroy();
	}
}
