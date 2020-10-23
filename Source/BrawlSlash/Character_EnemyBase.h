// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "Components/StaticMeshComponent.h"
#include "Character_EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class BRAWLSLASH_API ACharacter_EnemyBase : public ACharacter_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* attackCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* attackBoxStrong;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* shieldFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* shieldBack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


public:
	// Sets default values for this character's properties
	ACharacter_EnemyBase();
	
	UFUNCTION(BlueprintCallable)
	void BeginAttackCircle();

	UFUNCTION(BlueprintCallable)
	void EndAttackCircle();

	UFUNCTION(BlueprintCallable)
	void BeginAttackStrong();

	UFUNCTION(BlueprintCallable)
	void EndAttackStrong();

	void SetAttackState();

	float remainingTimeRecovery{ 0.f };

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "State")
	bool isInInnerCircle{ false };

	///////////////////////////////////////////////////////////////////////////
	//Attack
	bool needToAttackWeak{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownMin { 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownMax{ 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float currentAttackCoolDown{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ratioAttackWeak{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ratioAttackStrong{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int weakDamage{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int strongDamage{ 2 };
	///////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////
	//Defense
	bool ShieldCheckProtection(FVector attackerPos);
	void ShieldActivate();
	void ShieldDeActivate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldInFront {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldInBack {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldSwitchable {false};

	bool isNextShieldToActivateFront {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float angleAcceptanceForDefense {45};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeActive {0};

	float currentShieldTimeActive{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeToDeActivate {0};

	float currentShieldTimeToDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeDeActivate {0};

	float currentShieldTimeDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeToActivate {0};

	float currentShieldTimeToActivate{ 0 };
	///////////////////////////////////////////////////////////////////////////
};


