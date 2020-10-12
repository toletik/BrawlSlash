// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "Character_EnemyBase.h"
#include "MyGameInstance.h"
#include "Character_Player.generated.h"

UCLASS()
class BRAWLSLASH_API ACharacter_Player : public ACharacter_Base
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* cameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* followCamera;

	class ACharacter_EnemyBase* target = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int actualCombo = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Left Joystick
	void MoveForward(float Value);
	void MoveRight(float Value);

	//RightJoystick
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	//Buttons
	void Attack();
	void TakeHit(int damage) override;
	void Counter();
	void Execution();
	void StartAiming();
	void StopAiming();
	void Dodge();

	void TestRandomStart();
	void TestRandomEnd();

	UMyGameInstance* gameInstance;

	IInterface_Highlightable* elementToHighlight = nullptr;
	void SetElementToHighlight(IInterface_Highlightable* ptr)
	{
		if (elementToHighlight != nullptr)
			elementToHighlight->SetIfNeedToGlow(false);

		elementToHighlight = ptr;

		if (elementToHighlight != nullptr)
			elementToHighlight->SetIfNeedToGlow(true);
	};

	void UpdateElementToHighlight();

public:
	// Sets default values for this character's properties
	ACharacter_Player();

	ACharacter_EnemyBase* FocusedEnemy{ nullptr };

	FTimerHandle timerHandler;

	bool canCombo = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float comboTime = 1.0f;

	UFUNCTION()
	void StopCombo();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return cameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return followCamera; }


	//Camera Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float rotationSpeedHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float rotationSpeedVertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float verticalAngleMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float verticalAngleMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float distanceFromPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FRotator initialRotation {0, 0, 0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float positionLerpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float positionLerpLimitRange;

};
