// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interfaces/Interface_Damageable.h"
#include "Character_Base.generated.h"

UENUM(BlueprintType)
enum E_STATE
{
	IDLE,
	ATTACKING,
	PREPARING_DASH,
	DASHING_BACK,
	DASHING_HIT,
	DASH_ATTACK,
	PUSHED_BACK,
	PROJECTED,
	CINEMATIC,

	HITTED_WEAK,
	HITTED_STRONG,
	ATTACKING_WEAK,
	ATTACKING_STRONG,
	DEAD
};

UCLASS()
class BRAWLSLASH_API ACharacter_Base : public ACharacter, public IInterface_Damageable
{
	GENERATED_BODY() 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}
	
public:
	// Sets default values for this character's properties
	ACharacter_Base();

	virtual void TakeHit(int damage) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true"))
	float maxHealth = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true"))
	float health = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_STATE> state{E_STATE::IDLE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	class AMyAIDirector* currentEnemyGroup = nullptr;
};