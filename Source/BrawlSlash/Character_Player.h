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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Focus, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* focusDetector;

	TArray<AActor*> focusedActors;
		
	UFUNCTION()
	void FocusDetectorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void FocusDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int actualCombo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int firstComboDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int secondComboDamage = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int thirdComboDamage = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int dashHitDamage = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float minDistanceToDash = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float maxDistanceToDash = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float stickRange = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float stickPoint = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	float preparingDashDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	float dashBackRecoveryDuration = 1.0f;

	bool isGoingToStickPoint = false;

	bool hasChangedFocus = false;
	
	UMyGameInstance* gameInstance;

protected:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


	//Left Joystick
	void MoveForward(float Value);
	void MoveRight(float Value);

	//RightJoystick
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	//Buttons
	void StartTeleport(E_STATE teleportState);
	void StartBypass();
	void Bypass();
	void DashHit();

	void TurnAtRateFixed(float Rate);

	void GetNextFocus();
	void GetPreviousFocus();

	void TestRandomStart();
	void TestRandomEnd();

public:
	// Sets default values for this character's properties
	ACharacter_Player();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool isInSafeZone {false};

	virtual void TakeHit(int damage, E_STATE attackerState) override;

	void Attack();

	bool needToAttack = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class AMyAIDirector* currentEnemyGroup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	AActor* focus = nullptr;

	FTimerHandle timerHandler;

	bool canCombo = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float comboTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float jumpForceAfterNavDashHit = 1000.0f;

	UFUNCTION()
	void StopCombo();

	UFUNCTION(BlueprintCallable)
	void StopDashHit();

	UFUNCTION()
	void SetFocusNav(AActor* newFocus);
	
	void SetFocusToClosestFocus();
	void SetFocusToNextFocus();
	void SetFocusToPreviousFocus();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return cameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return followCamera; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Focus)
	float rotationSpeedWhenChangeFocus = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float knockbackForceAfterAttackBlocked = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float invincibleTime{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float currentInvincibleTime{0};


	//Camera OverAll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float rotationSpeedHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float rotationSpeedVertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float verticalAngleMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float verticalAngleMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	FRotator fixedRotation {0, 0, 0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float rotationSpeedHorizontalFixed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float positionLerpLimitRange;


	//Cam Nav
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float distanceNav;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float LerpSpeedNav;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float fovNav;

	void SetCameraStatsNav();

	//Cam LookAt
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float distanceLookAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float LerpSpeedLookAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float fovLookAt;

	void SetCameraStatsLookAt();

	//Cam Fight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float distanceFight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float LerpSpeedFight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float fovFight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	FRotator rotationForFight {0, 0, 0};

	void SetCameraStatsFight(FRotator rotationToAdopt);



	//////////////////////////////////////////////////////////////
	//DEBUG
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	float debugInnerCircleRadius = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	float debugBackCircleRadius = 800;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	bool isFocusInShortRange{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	class ACharacter_EnemyBase* debugNextFocus = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debug)
	class ACharacter_EnemyBase* debugPreviousFocus = nullptr;
};