// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interfaces/Interface_Damageable.h"
#include "Components/TimelineComponent.h"
#include "LDBrick_Door.generated.h"

UCLASS()
class BRAWLSLASH_API ALDBrick_Door : public AActor, public IInterface_Damageable
{
	GENERATED_BODY()
	
	// Sets default values for this actor's properties
	ALDBrick_Door();

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
	void OnTimelineUpdate();

	FTimerHandle timerAfterBreak;
	void Break();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* meshBoards{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* box{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* meshPoles{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* particles{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* audio{ nullptr };

	class ACharacter_Player* playerReference{ nullptr };

};
