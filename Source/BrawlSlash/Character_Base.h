// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interface_Highlightable.h"

#include "Character_Base.generated.h"

UENUM(BlueprintType)
enum E_STATE
{
	IDLE,
	ATTACKING,
	COUNTERING,
	EXECUTING,
	DODGING,


	HITTED_WEAK,
	HITTED_STRONG,
	HITTED_STUN,
	HITTED_RECO,
	ATTACKING_WEAK,
	ATTACKING_STRONG,
	COUNTERED,
	STUN,
	EXECUTED,
	DEAD
};

UCLASS()
class BRAWLSLASH_API ACharacter_Base : public ACharacter, public IInterface_Highlightable
{
	GENERATED_BODY()

	float health = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true"))
	float maxHealth = 5.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Attack();

public:
	// Sets default values for this character's properties
	ACharacter_Base();

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_STATE> state{E_STATE::IDLE};
};