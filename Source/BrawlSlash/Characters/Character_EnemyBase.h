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

	void TakeHit(int damage, E_STATE attackerState) override;

	//////////////////////////////////////////////////////////////
	//Custom event
	UFUNCTION(BlueprintImplementableEvent)
	void HitOther();

	UFUNCTION(BlueprintImplementableEvent)
	void ShieldHitted();

	UFUNCTION(BlueprintImplementableEvent)
	void LastEnemyInGroup();
	//////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable)
	void BeginAttackCircle();

	UFUNCTION(BlueprintCallable)
	void EndAttackCircle();

	UFUNCTION(BlueprintCallable)
	void BeginAttackStrong();

	UFUNCTION(BlueprintCallable)
	void EndAttackStrong();

	void SetAttackState();

	FTimerHandle timerHandler;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float stickRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float stickPoint = 60.0f;

	bool isRespectingAngularDist{ false };

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "State")
	bool isInInnerCircle{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool isAStrongEnemy{ false };

	///////////////////////////////////////////////////////////////////////////
	//Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float initialWaitAttack{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool needToAttackWeak{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownInnerMin { 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownInnerMax{ 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownBackMin{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float attackCoolDownBackMax{ 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float currentAttackCoolDown{ 0 };
	
	/////////////////////////////////////////////////////////////////
	//Will be removed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ratioAttackWeak{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ratioAttackStrong{ 0 };
	/////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int weakDamage{ 1 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int strongDamage{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float pushForceAfterBeingHit = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool attackStrongProject = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool attackCircleProject = true;
	///////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////
	//Defense
	bool ShieldCheckProtection(FVector attackerPos);
	void ShieldActivate();
	void ShieldDeActivate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float initialWaitShield{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldInFront {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldInBack {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldSwitchable {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isNextShieldToCycleFront {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float angleAcceptanceForDefense {45};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeActive {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeActive{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeToDeActivate {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeToDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeDeActivate {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeToActivate {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeToActivate{ 0 };
	///////////////////////////////////////////////////////////////////////////

	UFUNCTION()
	void LookAtPlayer();

	bool notLookAtPlayer = false;

	bool rotateDashedBack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	float rotateSpeedWhenBeingDashedBack{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
	float timeBeforeRotateWhenBeingDashedBack{ 2.0f };


	//Event
	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartNeedAttackWeak();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartAttackWeak();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartAttackStrong();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyStartFight();
};


