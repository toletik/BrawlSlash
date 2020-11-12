// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Called when the game starts or when spawned
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();

	player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	RunBehaviorTree(behaviorT);

	//Set Values for the BlackBoard
	Blackboard->SetValueAsFloat("radiusCloseToPlayer", radiusCloseToPlayer);
}

// Called every frame
void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Blackboard->SetValueAsVector("playerPos", player->GetActorLocation());
}
