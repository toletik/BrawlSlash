// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState_EnemyIsInStartup.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Character_Player.h"
#include "../Character_EnemyBase.h"

void UAnimNotifyState_EnemyIsInStartup::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(MeshComp->GetWorld(), 0));

	if (player)
	{
		ACharacter_EnemyBase* self = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());
		if (self)
			player->startupEnemy = self;
	}
}

void UAnimNotifyState_EnemyIsInStartup::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ACharacter_Player* player = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(MeshComp->GetWorld(), 0));

	if (player)
	{
		ACharacter_EnemyBase* self = Cast<ACharacter_EnemyBase>(MeshComp->GetOwner());
		if (player->startupEnemy && player->startupEnemy == self)
			player->startupEnemy = nullptr;
	}
}