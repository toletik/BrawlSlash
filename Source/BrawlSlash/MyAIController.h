// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API AMyAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, category = AI)
	UBehaviorTree* behaviorT;
	UPROPERTY(EditAnywhere, category = AI)
	float radiusCloseToPlayer;



	ACharacter* player;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UBlackboardComponent* GetBlackboard() { return Blackboard; };

	UPROPERTY(EditAnywhere, category = AI)
	float radiusInnerCircle;
	UPROPERTY(EditAnywhere, category = AI)
	float radiusBackCircle;
};
