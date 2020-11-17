// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "LDBrick_ShieldPoints.generated.h"

UCLASS()
class BRAWLSLASH_API ALDBrick_ShieldPoints : public AActor
{
	GENERATED_BODY()

	// Sets default values for this actor's properties
	ALDBrick_ShieldPoints();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector initialPos{ 0, 0, 0 };

	FTimeline timelineInitialHover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* curveFloatInitialHover;
	UFUNCTION()
	void OnTimelineInitialHoverUpdate();

	FTimeline timelineShapeUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* curveFloatShapeUp;
	UFUNCTION()
	void OnTimelineShapeUpUpdate();
	UFUNCTION()
	void OnTimelineShapeUpEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* outerSphere{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* innerSphere{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* sphereCollider{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* shieldPoints{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class USoundBase* shieldPointSound{ nullptr };

	class ACharacter_Player* playerReference{ nullptr };
};
