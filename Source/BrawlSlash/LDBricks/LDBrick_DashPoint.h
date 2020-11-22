// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LDBrick_DashPoint.generated.h"

UCLASS()
class BRAWLSLASH_API ALDBrick_DashPoint : public AActor
{
	GENERATED_BODY()
	
	// Sets default values for this actor's properties
	ALDBrick_DashPoint();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle timerForRespawn;

	void OnRepop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* outerSphere{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* innerSphere{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* waitForRepopSphere{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	FVector launchDirection{ 0, 0, 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	float launchForce{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	float timeBeforeRepop{ 6 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* glassBrokenSystem{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class USoundBase* glassBrokenSound{ nullptr };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
	class USoundBase* DashPointSpawnSound{ nullptr };

	class ACharacter_Player* playerReference{ nullptr };

public:
	void OnPlayerEndDash();
};
