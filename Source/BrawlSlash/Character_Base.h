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
	PREPARINGTELEPORT,
	BYPASSING,
	DASHING,
	PUSHED_BACK,

	HITTED_WEAK,
	HITTED_STRONG,
	HITTED_RECO,
	ATTACKING_WEAK,
	ATTACKING_STRONG,
	COUNTERED,
	DEAD
};

UCLASS()
class BRAWLSLASH_API ACharacter_Base : public ACharacter
{
	GENERATED_BODY() 



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* attackBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float health = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true"))
	float maxHealth = 5.0f;

public:
	// Sets default values for this character's properties
	ACharacter_Base();

	void TakeHit(int damage, E_STATE attackerState);

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<E_STATE> state{E_STATE::IDLE};

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Characteristics")
	bool isInFight{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	float walkSpeed{ 600 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	int toDoDamage{ 0 };

	UFUNCTION(BlueprintCallable)
	void BeginAttack();

	UFUNCTION(BlueprintCallable)
	void EndAttack();
};