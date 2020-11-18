// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_OnPlayerDeath.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../MyGameInstance.h"

void UAnimNotify_OnPlayerDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UMyGameInstance* gameInstance = Cast<UMyGameInstance>(MeshComp->GetOwner()->GetGameInstance());
	UGameplayStatics::OpenLevel(MeshComp->GetWorld(), FName(*UGameplayStatics::GetCurrentLevelName(MeshComp->GetWorld())));
}