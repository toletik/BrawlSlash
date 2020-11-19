// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component_EnemyShield.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class BRAWLSLASH_API UComponent_EnemyShield : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* shieldFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* shieldBack;

public:	
	// Sets default values for this component's properties
	UComponent_EnemyShield();

	void AttachToOwner();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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

	UFUNCTION(BlueprintImplementableEvent)
	void ShieldFrontHitted();

	UFUNCTION(BlueprintImplementableEvent)
	void ShieldBackHitted();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldFrontStartActive();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldFrontStartDeactive();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldFrontStartToActive();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldFrontStartToDeactive();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldBackStartActive();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldBackStartDeactive();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldBackStartToActive();

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyShieldBackStartToDeactive();
};
