// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LDBrick_KillZone.generated.h"

UCLASS()
class BRAWLSLASH_API ALDBrick_KillZone : public AActor
{
	GENERATED_BODY()

	// Sets default values for this actor's properties
	ALDBrick_KillZone();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	FRotator rotationForRespawn {0, 0, 0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* boxCollider;
	class ACharacter_Player* playerReference{ nullptr };
	class UMyGameInstance* gameInstanceReference{ nullptr };
};
