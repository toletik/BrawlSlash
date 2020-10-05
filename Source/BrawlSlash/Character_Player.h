// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
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
	void Counter();
	void Execution();
	void Dodge();

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

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float baseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float baseLookUpRate;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return cameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return followCamera; }
};
