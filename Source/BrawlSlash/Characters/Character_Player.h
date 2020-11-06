// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Base.h"
#include "../MyGameInstance.h"
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
	class UStaticMeshComponent* detectorOfFocus;

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
	float stickPointFight = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float stickPointNav = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float stickRangeFight = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float stickRangeNav = 350.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float maxDistanceToDashFight = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float maxDistanceToDashNav = 10000.0f;

	FVector dashHitImpactPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	float preparingDashDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mobility Points", meta = (AllowPrivateAccess = "true"))
	float dashBackRecoveryDuration = 1.0f;

	bool isGoingToStickPoint = false;
	
	bool needToRefreshCameraBehind {true};

	UMyGameInstance* gameInstance;

	FTimerHandle dashBackCooldownTimer;

	bool isDashBackInCooldown = false;

	void StopDashBackRecovery();

	void LookAtFocus(bool lerp);

	void DashAttack();

	void UpdateTimers();

	void UpdateDebug();

	void UpdateCamera();

	void UpdatePosToStickPoint();

	void UpdateDashingHit();

	void UpdateDashingBack();

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
	void StartDash(E_STATE dashState);
	void StartDashBack();
	void DashBack();
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

	void TakeHit(int damage, E_STATE attackerState) override;

	void Attack();

	bool needToAttack = false;

	bool isAtEndOfAttackAnim = false;

	FTimerHandle comboTimer;

	FTimerHandle dashTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	AActor* focus = nullptr;

	bool canCombo = false;

	bool canCancelCombo = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float comboTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float jumpForceAfterNavDashHit = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	FVector jumpVectorAfterNavDashHit = FVector::UpVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Characteristics)
	float invincibleTime{ 1 };

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Characteristics)
	float currentInvincibleTime{ 0 };

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

	//Camera OverAll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float rotationSpeedHorizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float rotationSpeedVertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float LagSpeedCameraToPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float LagPositionCameraToPlayerLimitRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraOverAll)
	float rotationLerpFactorJoystick{2};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraOverAll)
	FRotator cameraRotation {0, 0, 0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraOverAll)
	FVector cameraPosition {0, 0, 0};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraOverAll)
	float cameraLength{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraOverAll)
	float cameraFOV{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraOverAll)
	float cameraVerticalAngleMin{ 0 };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraOverAll)
	float cameraVerticalAngleMax{ 0 };

	//Cam Nav
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float distanceNav;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float verticalAngleMinNav {10};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float verticalAngleMaxNav {80};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float fovNav;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNav)
	float cameraHeightNav{ 0 };

	void SetCameraStatsNav();

	//Cam fixedRotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNavBehind)
	float fixedRotationAngle {45};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNavBehind)
	float rotationLerpFactorFixedRotation{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNavBehind)
	float timeForComeBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraNavBehind)
	float currentTimeForComeBack;

	//Cam LookAt
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float rotationLerpFactorToLookAt{ 2 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float distanceLookAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float verticalAngleMinLookAt {10};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float verticalAngleMaxLookAt {80};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float fovLookAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraLookAt)
	float cameraHeightLookAt{ 0 };

	void SetCameraStatsLookAt();

	//Cam Fight
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float distanceFight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float verticalAngleMinFight {10};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float verticalAngleMaxFight {80};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float fovFight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float cameraHeightFight{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float zoomDezoomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float mindistance{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CameraFight)
	float maxdistance{2000};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = CameraFight)
	FRotator rotationForFight {0, 0, 0};

	void SetCameraStatsFight(FRotator rotationToAdopt);

	//Events
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerStartFight();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerEndFight();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerStartIsProjected();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerEndIsProjected();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerStartInvincibleTime();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerEndInvincibleTime();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerStartHitted();

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

	FVector testForDashBack{ 0, 0, 0 };
};