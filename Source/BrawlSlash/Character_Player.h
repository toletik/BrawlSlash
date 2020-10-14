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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int actualCombo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float timeToStartAiming = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int firstComboDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int secondComboDamage = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int thirdComboDamage = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	int maxMobilityPoints = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	int onAttackMobilityPoints = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	int onDodgeMobilityPoints = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	int onTpHitMobilityPoints = 2;

	int currentMobilityPoints;

	class USceneComponent*			coneJoint;
	class UStaticMeshComponent*		coneMesh;
	class UMaterialInstanceDynamic* coneMat;
	TArray<AActor*>					overlappedTargets;

	UFUNCTION()
	void ConeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void ConeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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
	void TakeHit(int damage) override;
	void Execution();
	void StartTeleport();
	void StopTeleport();
	void StartAiming();

	void TestRandomStart();
	void TestRandomEnd();

	UMyGameInstance* gameInstance;

	void UpdateTarget();

public:
	// Sets default values for this character's properties
	ACharacter_Player();

	void Attack();

	bool needToAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	AActor* focus = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	AActor* target = nullptr;

	FTimerHandle timerHandler;

	bool canCombo = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float comboTime = 1.0f;

	UFUNCTION()
	void StopCombo();

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeFocus();
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float fightAngle = 45.0f;
};

