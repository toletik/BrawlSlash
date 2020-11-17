// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Interface_Damageable.h"
#include "Components/TimelineComponent.h"
#include "LDBrick_ShieldPoints.h"
#include "LDBrick_Chest.generated.h"

UCLASS()
class BRAWLSLASH_API ALDBrick_Chest : public AActor, public IInterface_Damageable
{
	GENERATED_BODY()
	
	// Sets default values for this actor's properties
	ALDBrick_Chest();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeHit(int damage) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	float maxHealth = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	float health = 0.0f;

	FVector initalMeshScale{ 0, 0, 0 };

	FTimeline timelineShapeUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* curveFloat;

	UFUNCTION()
	void OnTimelineUpdate(float value);

	FTimerHandle timerAfterBreak;
	void Break();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* root{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* mesh{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* particles{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* audio{ nullptr };

	class ACharacter_Player* playerReference{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ALDBrick_ShieldPoints> shieldPointClassType;
};
