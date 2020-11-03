// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_StartCanCombo.h"
#include "../Characters/Character_Player.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UAnimNotify_StartCanCombo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* character = Cast<ACharacter_Player>(MeshComp->GetOwner());
	if (character)
	{
		character->canCombo = true;
		character->GetWorldTimerManager().ClearTimer(character->timerHandler);
		character->GetWorldTimerManager().SetTimer(character->timerHandler, character, &ACharacter_Player::StopCombo, character->comboTime, false);
	}
}