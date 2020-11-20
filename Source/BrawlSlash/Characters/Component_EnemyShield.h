// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component_EnemyShield.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCustomEventSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BRAWLSLASH_API UComponent_EnemyShield : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponent_EnemyShield();

	class UStaticMeshComponent* shieldFront;

	class UStaticMeshComponent* shieldBack;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void OnOwnerDeath();

	bool ShieldCheckProtection(FVector attackerPos);
	void ShieldActivate();
	void ShieldDeActivate();
	void ShieldToActivate();
	void ShieldToDeActivate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float initialWaitShield{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldInFront{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldInBack{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isShieldSwitchable{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	bool isNextShieldToCycleFront{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float angleAcceptanceForDefense{ 45 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeActive{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeActive{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeToDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeToDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeDeActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float shieldTimeToActivate{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense")
	float currentShieldTimeToActivate{ 0 };

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature ShieldFrontHitted;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature ShieldBackHitted;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldFrontStartActive;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldFrontStartDeactive;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldFrontStartToActive;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldFrontStartToDeactive;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldBackStartActive;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldBackStartDeactive;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldBackStartToActive;

	UPROPERTY(BlueprintAssignable)
	FCustomEventSignature EnemyShieldBackStartToDeactive;
};
