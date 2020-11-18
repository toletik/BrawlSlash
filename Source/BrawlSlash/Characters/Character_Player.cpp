 // Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Player.h"
#include "../AI/MyAIDirector.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Character_EnemyBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

#include "Camera/PlayerCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "../MyGameInstance.h"
#include "../interfaces/Interface_Damageable.h"
#include "../LDBricks/LDBrick_DashPoint.h"
#include "LevelSequenceActor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

// Sets default values
ACharacter_Player::ACharacter_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller


	// Create a follow camera
	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera")); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName); 
	followCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	
	detectorOfFocus = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FocusDetector"));
	detectorOfFocus->SetupAttachment(RootComponent);
}

void ACharacter_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	//Left Joystick
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacter_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacter_Player::MoveRight);

	//RightJoystick
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacter_Player::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacter_Player::LookUpAtRate);

	//Buttons
	PlayerInputComponent->BindAction("DashBack", IE_Pressed, this, &ACharacter_Player::OnAPressed);
	PlayerInputComponent->BindAction("DashBack", IE_Released, this, &ACharacter_Player::OnAReleased);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacter_Player::Attack);
	PlayerInputComponent->BindAction("FocusNextEnemy", IE_Pressed, this, &ACharacter_Player::GetNextFocus);
	PlayerInputComponent->BindAction("FocusPreviousEnemy", IE_Pressed, this, &ACharacter_Player::GetPreviousFocus);


	PlayerInputComponent->BindAction("TestRandom", IE_Pressed, this, &ACharacter_Player::TestRandomStart);
	PlayerInputComponent->BindAction("TestRandom", IE_Released, this, &ACharacter_Player::TestRandomEnd);
}

// Called when the game starts or when spawned
void ACharacter_Player::BeginPlay()
{
	Super::BeginPlay();
	
	gameInstance = Cast<UMyGameInstance>(GetGameInstance());

	//init values of camera
	SetCameraStatsNav();
	cameraRotation = FRotationMatrix::MakeFromX(GetActorForwardVector().RotateAngleAxis(fixedRotationAngle, GetActorRightVector())).Rotator(),
	Controller->SetControlRotation(cameraRotation);
	cameraBoom->SetRelativeLocation(cameraPosition);
	cameraBoom->TargetArmLength = cameraLength;
	cameraBoom->CameraLagMaxDistance = LagPositionCameraToPlayerLimitRange;
	cameraBoom->CameraLagSpeed = LagSpeedCameraToPlayer;
	followCamera->SetFieldOfView(cameraFOV);
	cameraBoom->bEnableCameraLag = true; 
	currentTimeForComeBack = timeForComeBack;

	attackBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Player::AttackOverlap);

	//Focus Detector
	detectorOfFocus->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Player::FocusDetectorBeginOverlap);
	detectorOfFocus->OnComponentEndOverlap.AddDynamic(this, &ACharacter_Player::FocusDetectorEndOverlap);

	if (gameInstance->hasRestartLevel && sequenceToPlayOnRestart)
	{
		ALevelSequenceActor* temp;
		currentSequence = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequenceToPlayOnRestart, FMovieSceneSequencePlaybackSettings(), temp);
		currentSequence->Play();
	}
}

// Called every frame
void ACharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTimers(DeltaTime);

	UpdateDebug();

	UpdateCamera();

	//Look at focus while idle
	if ((GetVelocity().Size() < 0.5f || state == E_STATE::ATTACKING) && !isInReco) 
		LookAtFocus(true);

	UpdatePosToStickPoint();

	UpdateDashingHit();

	UpdateDashingBack();

	CheckGround();
}

void ACharacter_Player::UpdateTimers(float deltaTime)
{
	if (currentInvincibleTime > 0)
	{
		currentInvincibleTime -= deltaTime;

		if (currentInvincibleTime <= 0)
			PlayerEndInvincibleTime();
	}

	currentTimeForComeBack += deltaTime;

	if (currentSequence && !currentSequence->IsPlaying())
	{
		currentSequence = nullptr;
		isHoldingInputToPassSequence = false;
		currentTimeToPassSequence = 0.0f;
		isSequenceSkippable = false;
		state = IDLE;
	}

	if (isHoldingInputToPassSequence)
	{
		currentTimeToPassSequence += deltaTime;
		if (currentTimeToPassSequence >= timeToPassSequence)
		{
			isHoldingInputToPassSequence = false;
			isSequenceSkippable = false;
			currentTimeToPassSequence = 0.0f;
			currentSequence->GoToEndAndStop();
			currentSequence = nullptr;
			state = IDLE;
		}
	}
}
void ACharacter_Player::UpdateDebug()
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(focus);
	/*
	if (enemy)
	//&& (focus->GetActorLocation() - GetActorLocation()).Size() > enemy->minDistanceToPlayerDash 
	//&& (focus->GetActorLocation() - GetActorLocation()).Size() < enemy->maxDistanceToPlayerDash)
		isFocusInShortRange = true;
	else if (focus 
		 //&& (focus->GetActorLocation() - GetActorLocation()).Size() > minDistanceToDashNav 
		 && (focus->GetActorLocation() - GetActorLocation()).Size() < maxDistanceToDashNav)
		isFocusInShortRange = true;
	else
		isFocusInShortRange = false;
		*/
	debugNextFocus = nullptr;
	debugPreviousFocus = nullptr;
	if (currentEnemyGroup)
	{
		currentEnemyGroup->SetDebugFocusToNextEnemy();
		currentEnemyGroup->SetDebugFocusToPreviousEnemy();
	}

	GEngine->AddOnScreenDebugMessage(-97, 1.0f, FColor::Cyan, FString("next ").Append(GetDebugName(debugNextFocus)));
	GEngine->AddOnScreenDebugMessage(-98, 1.0f, FColor::Cyan, FString("previous ").Append(GetDebugName(debugPreviousFocus)));
}
void ACharacter_Player::UpdateCamera()
{
	//clean by factoring by RInterpTo

	//Camera Fight
	if (currentEnemyGroup)
	{
		if (currentTimeForComeBack >= timeForComeBack)
		{
			cameraRotation = rotationForFight;
			Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), cameraRotation, GetWorld()->GetDeltaSeconds(), rotationLerpFactorFixedRotation));
		}

		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), cameraRotation, GetWorld()->GetDeltaSeconds(), rotationLerpFactorJoystick));
	
		// Zoom / deZoom
		if (!currentEnemyGroup->IsAllEnemiesInCameraSight(GetWorld()->GetFirstPlayerController()))
			cameraLength = FMath::Min(maxDistanceCameraFight, cameraLength + zoomDezoomSpeed * GetWorld()->GetDeltaSeconds());
		else
			cameraLength = FMath::Max(minDistanceCameraFight, cameraLength - zoomDezoomSpeed * GetWorld()->GetDeltaSeconds());
	}
	//Camera LookAt
	else if (focus)
	{
		cameraRotation = FRotationMatrix::MakeFromX(focus->GetActorLocation() - GetActorLocation() - GetActorUpVector() * 500).Rotator();
		cameraRotation.Pitch = FMath::Clamp(cameraRotation.Pitch, cameraVerticalAngleMax, cameraVerticalAngleMin);
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), cameraRotation, GetWorld()->GetDeltaSeconds(), rotationLerpFactorToLookAt));
	
	}
	//Camera Behind
	else if (currentTimeForComeBack >= timeForComeBack && needToRefreshCameraBehind)
	{
		cameraRotation = FRotationMatrix::MakeFromX(GetActorForwardVector().RotateAngleAxis(fixedRotationAngle, GetActorRightVector())).Rotator();
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), cameraRotation, GetWorld()->GetDeltaSeconds(), rotationLerpFactorFixedRotation));
	}
	//Camera Nav
	else
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), cameraRotation, GetWorld()->GetDeltaSeconds(), rotationLerpFactorJoystick));



	cameraBoom->SetRelativeLocation( FMath::Lerp(cameraBoom->GetRelativeLocation(), cameraPosition, GetWorld()->GetDeltaSeconds()) );
	cameraBoom->TargetArmLength = FMath::Lerp(cameraBoom->TargetArmLength, cameraLength, GetWorld()->GetDeltaSeconds());
	followCamera->SetFieldOfView(FMath::Lerp(followCamera->FieldOfView, cameraFOV, GetWorld()->GetDeltaSeconds()) );

}
void ACharacter_Player::UpdatePosToStickPoint()
{
	//if player is in fight and he is too far to hit his focus
	if (focus && state == E_STATE::ATTACKING && !isInReco)
	{
		FVector direction = GetStickPoint() - GetActorLocation();
		direction.Z = 0;
		AddMovementInput(direction, 1.0f);
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
		GetCharacterMovement()->bOrientRotationToMovement = true;
}
void ACharacter_Player::UpdateDashingHit()
{
	if (state == E_STATE::DASHING_HIT && FVector::DotProduct(dashPosToReach - (GetActorLocation() + GetVelocity() * GetWorld()->GetDeltaSeconds()) , dashDirection) < 0)
		StopDashHit();

}
void ACharacter_Player::UpdateDashingBack()
{
	if (state == E_STATE::DASHING_BACK && FVector::DotProduct(dashPosToReach - (GetActorLocation() + GetVelocity() * GetWorld()->GetDeltaSeconds()), dashDirection) < 0)
		StopDashBack();
}

void ACharacter_Player::CheckGround()
{
	if (state != IDLE || GetCharacterMovement()->IsFalling())
		return;

	FHitResult hit;
	FCollisionQueryParams raycastParams;
	raycastParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() - FVector::UpVector * 100.0f, ECC_WorldStatic, raycastParams);

	if (hit.GetActor() && hit.GetActor()->ActorHasTag("Stone"))
		isOnStone = true;
	else
		isOnStone = false;
}

void ACharacter_Player::FocusDetectorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Focusable"))
	{
		focusedActors.Add(OtherActor);

		if (!currentEnemyGroup && state == E_STATE::IDLE)
			//SetFocusNav(OtherActor);
			SetFocusToClosestFocus();

	}
}

void ACharacter_Player::FocusDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Focusable"))
	{
		focusedActors.Remove(OtherActor);

		if (!currentEnemyGroup && focus == OtherActor)
			SetFocusToClosestFocus();

		if(!focus)
			currentTimeForComeBack = timeForComeBack;
	}
}

void ACharacter_Player::AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
		return;

	IInterface_Damageable* damageableActor = Cast<IInterface_Damageable>(OtherActor);

	if (damageableActor)
	{
		ACharacter_EnemyBase* enemyCast = Cast<ACharacter_EnemyBase>(OtherActor);

		if (enemyCast && enemyCast->ShieldCheckProtection(GetActorLocation()))
		{
			PlayerHitShield();
			state = E_STATE::PUSHED_BACK;
			StopCombo();
			LaunchCharacter(-GetActorForwardVector() * knockbackForceAfterAttackBlocked, true, true);

			if (actualCombo == 3)
				isInReco = false;

			return;
		}

		damageableActor->TakeHit(toDoDamage);
		EndAttack();
	}

}

void ACharacter_Player::TakeHit(int damage)
{
	if (currentInvincibleTime > 0)
		return;

	if (currentShieldPoint > 0)
	{
		currentShieldPoint--;
		PlayerLostShieldPoint();
		currentInvincibleTime = invincibleTime;
		if (currentShieldPoint == 0)
			PlayerLostAllShieldPoints();

		return;
	}

	Super::TakeHit(damage);

	if (health > 0)
	{
		currentInvincibleTime = invincibleTime;
		PlayerStartInvincibleTime();
	}
	else
	{
		DisableInput(GetWorld()->GetFirstPlayerController());
		gameInstance->numberOfPlayerShieldPoints = 0;
		gameInstance->hasRestartLevel = true;
		if (sequenceToPlayOnDeath)
		{
			ALevelSequenceActor* temp;
			currentSequence = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), sequenceToPlayOnDeath, FMovieSceneSequencePlaybackSettings(), temp);
			currentSequence->Play();
		}
	}
}

//Left Joystick
void ACharacter_Player::MoveForward(float Value)
{
	needToRefreshCameraBehind = true;

	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (canCancelCombo)
		{
			state = E_STATE::IDLE;
			canCancelCombo = false;
			isInReco = false;
		}

		if (state == E_STATE::IDLE)
		{
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}

		if (Value == -1)
			needToRefreshCameraBehind = false;
	}
}
void ACharacter_Player::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (canCancelCombo)
		{
			state = E_STATE::IDLE;
			canCancelCombo = false;
			isInReco = false;
		}

		if (state == E_STATE::IDLE)
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, Value);
		}
	}
}

//RightJoystick
void ACharacter_Player::TurnAtRate(float Rate)
{
	if (Rate != 0.0f)
	{
		currentTimeForComeBack = 0.0f;
		// calculate delta for this frame from the rate information	
		cameraRotation.Yaw += Rate * rotationSpeedHorizontal * GetWorld()->GetDeltaSeconds() * (gameInstance->isXRevert ? -1.0f : 1.0f);
	}
}
void ACharacter_Player::LookUpAtRate(float Rate)
{

	if (Rate != 0.0f)
	{
		currentTimeForComeBack = 0.0f;
		// calculate delta for this frame from the rate information

		cameraRotation.Pitch += Rate * rotationSpeedVertical * GetWorld()->GetDeltaSeconds() * (gameInstance->isYRevert ? 1.0f : -1.0f);
		cameraRotation.Pitch = FMath::Clamp(cameraRotation.Pitch, cameraVerticalAngleMax, cameraVerticalAngleMin);
	}
}

void ACharacter_Player::DashAttack()
{
	//called after stop dashhit
	LookAtFocus(false);
	toDoDamage = dashHitDamage;
	state = E_STATE::DASH_ATTACK;
}

//Buttons
void ACharacter_Player::Attack()
{
	//buffer for next combo
	if (state == E_STATE::ATTACKING)
	{	
		//Start dash
		if (CheckIfCanDash() && !isDashHitInCooldown ) 
			StartDash(E_STATE::DASHING_HIT);
		else if (canCombo)
			needToAttack = true;
	}
	else if (state == E_STATE::IDLE)
	{
		//Start dash
		if (CheckIfCanDash() && !isDashHitInCooldown)
			StartDash(E_STATE::DASHING_HIT);
		//Attack
		else if (!GetCharacterMovement()->IsFalling())
		{
			//Start attack
			state = E_STATE::ATTACKING;
			LookAtFocus(false);

			//attack combo 2 or 3
			if (canCombo)
				SetNextAttackCombo();
			//attack combo 1
			else
			{
				actualCombo = 1;
				toDoDamage = firstComboDamage;
				PlayerCombo1();
			}
		}
	}
}

void ACharacter_Player::SetNextAttackCombo()
{
	StopCombo();
	actualCombo++;

	if (actualCombo == 2)
	{
		toDoDamage = secondComboDamage;
		PlayerCombo2();
	}
	else
	{
		toDoDamage = thirdComboDamage;
		PlayerCombo3();
	}
}

bool ACharacter_Player::CheckIfCanDash()
{
	if (!focus)
		return false;
	if (focus->ActorHasTag("DashPoint"))
		return true;

	FHitResult hit;
	FCollisionQueryParams raycastParams;
	raycastParams.AddIgnoredActor(this);

	if (currentEnemyGroup)
		for (int i = 0; i < currentEnemyGroup->enemies.Num(); ++i)
			if (currentEnemyGroup->enemies[i] != focus)
				raycastParams.AddIgnoredActor(currentEnemyGroup->enemies[i]);

	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), focus->GetActorLocation(), ECC_WorldStatic, raycastParams);

	float distanceToFocus = (hit.Location - GetActorLocation()).Size();

	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(focus);
	//if focus is target in world or focus is an enemy or not and player is at the good distance to dash, then dash
	if ((enemy && minToDashFight < distanceToFocus) || (!enemy && minToDashNav < distanceToFocus))
		return true;
	
	return false;
}

void ACharacter_Player::OnAPressed()
{
	if (isSequenceSkippable)
		isHoldingInputToPassSequence = true;
	if (currentEnemyGroup && !isDashBackInCooldown && (state == E_STATE::IDLE || state == E_STATE::ATTACKING) && Cast<ACharacter_EnemyBase>(focus))
		StartDash(E_STATE::DASHING_BACK);
}

void ACharacter_Player::OnAReleased()
{
	if (isSequenceSkippable)
	{
		isHoldingInputToPassSequence = false;
		currentTimeToPassSequence = 0.0f;
	}
}

void ACharacter_Player::StartDash(E_STATE teleportState)
{
	state = E_STATE::PREPARING_DASH;
	GetWorldTimerManager().ClearTimer(dashTimer);
	EndAttack();
	StopCombo();

	if (teleportState == E_STATE::DASHING_BACK)
		GetWorldTimerManager().SetTimer(dashTimer, this, &ACharacter_Player::DashBack, preparingDashDuration, false);
	else if (teleportState == E_STATE::DASHING_HIT)
		GetWorldTimerManager().SetTimer(dashTimer, this, &ACharacter_Player::DashHit, preparingDashDuration, false);
}

void ACharacter_Player::DashHit()
{
	if (!focus)
	{
		state = E_STATE::IDLE;
		return;
	}

	dashPosToReach = GetStickPoint();

	//Check if there is free space when the player is going to dash
	if (!CheckIfFreeSpaceForDash())
	{
		state = E_STATE::IDLE;
		return;
	}
;
	state = E_STATE::DASHING_HIT;
	GetWorldTimerManager().ClearTimer(dashTimer);
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	//collision only need to be disable in fight
	if (currentEnemyGroup)
		SetActorEnableCollision(false);

	FVector direction = dashPosToReach - GetActorLocation();
	direction.Normalize();
	dashDirection = direction;
	LaunchCharacter(direction * 10000.0f, true, true);

	initialFocus = focus;
}

void ACharacter_Player::DashBack()
{
	if (!focus)
	{
		state = E_STATE::IDLE;
		return;
	}
	
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(focus);

	if (FVector::DotProduct(focus->GetActorLocation() - GetActorLocation(), focus->GetActorForwardVector()) < 0)
		dashPosToReach = focus->GetActorLocation() - focus->GetActorForwardVector() * ((enemy->GetCapsuleComponent()->GetScaledCapsuleRadius()) + stickPointFight);
	else
		dashPosToReach = focus->GetActorLocation() + focus->GetActorForwardVector() * ((enemy->GetCapsuleComponent()->GetScaledCapsuleRadius()) + stickPointFight);
	dashPosToReach.Z = GetActorLocation().Z;

	//Check if there is free space when the player is going to dash
	if (!CheckIfFreeSpaceForDash())
	{
		state = E_STATE::IDLE;
		return;
	}

	state = E_STATE::DASHING_BACK;
	GetWorldTimerManager().ClearTimer(dashTimer);
	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	SetActorEnableCollision(false);

	FVector direction = dashPosToReach - GetActorLocation();
	direction.Normalize();
	dashDirection = direction;
	LaunchCharacter(direction * 10000.0f, true, true);
}

FVector	ACharacter_Player::GetStickPoint()
{
	FVector vectorToReturn = FVector::ZeroVector;
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(focus);


	//if player is in fight
	if (enemy)
	{
		if (FVector::DotProduct(focus->GetActorLocation() - GetActorLocation(), focus->GetActorForwardVector()) < 0)
			vectorToReturn = focus->GetActorLocation() + focus->GetActorForwardVector() * ((enemy->GetCapsuleComponent()->GetScaledCapsuleRadius()) + stickPointFight);
		else
			vectorToReturn = focus->GetActorLocation() - focus->GetActorForwardVector() * ((enemy->GetCapsuleComponent()->GetScaledCapsuleRadius()) + stickPointFight);

		vectorToReturn.Z = GetActorLocation().Z;

	}
	//if focus is a Door
	else if (focus->ActorHasTag("Door"))
	{
		FHitResult hit;
		FCollisionQueryParams raycastParams;
		raycastParams.AddIgnoredActor(this);
		FVector focusPos = focus->GetActorLocation();
		focusPos.Z = GetActorLocation().Z;
		GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), focusPos, ECC_WorldDynamic, raycastParams);

		if (hit.GetActor() != nullptr && hit.GetActor()->ActorHasTag("Door"))
		{
			FVector hitLocation = hit.Location;
			hitLocation.Z = GetActorLocation().Z;
			FVector focusToHit = (hitLocation - focusPos);
			focusToHit.Normalize();
			vectorToReturn = hitLocation + focusToHit * stickPointNav;
		}
	}
	//if focus is a Chest
	else if (focus->ActorHasTag("Chest"))
	{
		FHitResult hit;
		FCollisionQueryParams raycastParams;
		raycastParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), focus->GetActorLocation(), ECC_WorldDynamic, raycastParams);

		if (hit.GetActor() != nullptr && hit.GetActor()->ActorHasTag("Chest"))
		{
			FVector focusToHit = (hit.Location - focus->GetActorLocation());
			focusToHit.Normalize();
			vectorToReturn = hit.Location + focusToHit * stickPointNav;
			vectorToReturn.Z = focus->GetActorLocation().Z;
		}
	}
	//if focus is a DashPoint
	else if (focus->ActorHasTag("DashPoint"))
		vectorToReturn = focus->GetActorLocation();

	return vectorToReturn;
}

bool ACharacter_Player::CheckIfFreeSpaceForDash()
{
	//Check if there is free space when the player is going to dash
	FHitResult hit;
	FCollisionQueryParams raycastParams;
	raycastParams.AddIgnoredActor(focus);
	FVector focusPos = focus->GetActorLocation();
	focusPos.Z = GetActorLocation().Z;
	GetWorld()->LineTraceSingleByChannel(hit, focusPos, dashPosToReach, ECC_WorldStatic, raycastParams);
	DrawDebugLine(GetWorld(), focusPos, dashPosToReach, FColor::Blue, false, 1, 0, 5);

	if (hit.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(-44, 0.2f, FColor::Red, GetDebugName(hit.GetActor()));
		return false;
	}


	return true;
}

void ACharacter_Player::StopDashHit()
{
	SetActorLocation(dashPosToReach);

	//called at the end of dashhit
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	if (initialFocus->ActorHasTag("DashPoint"))
	{
		state = IDLE;
		PlayerStopDashHitDashPoint();

		ALDBrick_DashPoint* dashPoint = Cast<ALDBrick_DashPoint>(initialFocus);
		dashPoint->OnPlayerEndDash();

		if (currentEnemyGroup)
		{
			initialFocus->Destroy();
			SetActorEnableCollision(true);
			currentEnemyGroup->SetFocusToClosestEnemy();
			currentEnemyGroup->UpdateIfIsInInner();
		}
	}
	else
	{
		if (currentEnemyGroup)
		{
			SetActorEnableCollision(true);
			currentEnemyGroup->UpdateIfIsInInner();
		}

		isDashHitInCooldown = true;
		GetWorldTimerManager().SetTimer(dashHitCooldownTimer, this, &ACharacter_Player::StopDashHitRecovery, dashHitRecoveryDuration, false);
		DashAttack();
	}
}

void ACharacter_Player::StopDashBack()
{
	SetActorLocation(dashPosToReach);
	state = E_STATE::IDLE;
	LookAtFocus(false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	SetActorEnableCollision(true);
	GetCharacterMovement()->Velocity = FVector::ZeroVector;

	isDashBackInCooldown = true;
	GetWorldTimerManager().SetTimer(dashBackCooldownTimer, this, &ACharacter_Player::StopDashBackRecovery, dashBackRecoveryDuration, false);

	currentEnemyGroup->UpdateIfIsInInner();
	ACharacter_EnemyBase* enemyFocus = Cast<ACharacter_EnemyBase>(focus);
	enemyFocus->beingDashedBack = true;
	GetWorldTimerManager().SetTimer(enemyFocus->timerHandler, enemyFocus, &ACharacter_EnemyBase::LookAtPlayer, enemyFocus->timeBeforeRotateWhenBeingDashedBack, false);

}

void ACharacter_Player::StopCombo()
{
	GetWorldTimerManager().ClearTimer(comboTimer);
	canCombo = false;
	needToAttack = false;
}

void ACharacter_Player::SetFocusNav(AActor* newFocus)
{
	//when set to !nullptr call overlap end
	focus = newFocus;
	
	if (!currentEnemyGroup)
	{
		if (focus)
			SetCameraStatsLookAt();
		else
			SetCameraStatsNav();
	}
}

void ACharacter_Player::TestRandomStart()
{
	if (focusedActors.Num() > 0)
		focus = focusedActors[0];

	GEngine->AddOnScreenDebugMessage(-17, 1.0f, FColor::Cyan, FString::FromInt(focusedActors.Num()) );

}
void ACharacter_Player::TestRandomEnd()
{
	GEngine->AddOnScreenDebugMessage(-17, 1.0f, FColor::Cyan, "End");
}

void ACharacter_Player::GetNextFocus()
{
	if (state == E_STATE::PREPARING_DASH || state == E_STATE::DASHING_BACK || state == E_STATE::DASHING_HIT)
		return;

	if (currentEnemyGroup)
		currentEnemyGroup->SetFocusToNextEnemy();
	else if (focus)
		SetFocusToNextFocus();
}
void ACharacter_Player::GetPreviousFocus()
{
	if (state == E_STATE::PREPARING_DASH || state == E_STATE::DASHING_BACK || state == E_STATE::DASHING_HIT)
		return;

	if (currentEnemyGroup)
		currentEnemyGroup->SetFocusToPreviousEnemy();
	else if (focus)
		SetFocusToPreviousFocus();
}

void ACharacter_Player::SetCameraStatsNav()
{
	cameraVerticalAngleMin = -verticalAngleMinNav;
	cameraVerticalAngleMax = -verticalAngleMaxNav;
	cameraPosition = { 0, 0, cameraHeightNav };
	cameraLength = distanceNav;
	cameraFOV = fovNav;
}
void ACharacter_Player::SetCameraStatsLookAt()
{
	cameraVerticalAngleMin = -verticalAngleMinLookAt;
	cameraVerticalAngleMax = -verticalAngleMaxLookAt;
	cameraPosition = { 0, 0, cameraHeightLookAt };
	cameraLength = distanceLookAt;
	cameraFOV = fovLookAt;
}
void ACharacter_Player::SetCameraStatsFight(FRotator rotationToAdopt, float minDistanceToAdopt, float maxDistanceToAdopt)
{
	minDistanceCameraFight = minDistanceToAdopt;
	maxDistanceCameraFight = maxDistanceToAdopt;
	cameraVerticalAngleMin = -verticalAngleMinFight;
	cameraVerticalAngleMax = -verticalAngleMaxFight;
	rotationForFight = rotationToAdopt;
	cameraPosition = { 0, 0, cameraHeightFight };
	cameraLength = distanceFight;
	cameraFOV = fovFight;

	currentTimeForComeBack = timeForComeBack;
}

void ACharacter_Player::SetFocusToClosestFocus()
{
	//player will get a focus if focusedActors.Num > 0
	SetFocusNav(nullptr);

	FVector playerPos = GetActorLocation();
	float distanceFromPlayer{ INFINITY };


	for (int i = 0; i <= focusedActors.Num() - 1; ++i)
	{
		float distanceToFocus = (focusedActors[i]->GetActorLocation() - playerPos).Size();
		if (distanceToFocus < distanceFromPlayer)
		{
			distanceFromPlayer = distanceToFocus;
			SetFocusNav(focusedActors[i]);
		}
	}
}

void ACharacter_Player::SetFocusToNextFocus()
{
	FVector playerPos = GetActorLocation();
	FVector vectorReference = focus->GetActorLocation() - playerPos;
	float smallestAngle = 360;

	for (int i = 0; i < focusedActors.Num(); ++i)
	{
		if (focusedActors[i] != focus)
		{
			FVector playerToFocus = focusedActors[i]->GetActorLocation() - playerPos;
			float FocusAngle = (FVector::CrossProduct(vectorReference, playerToFocus).Z > 0) ? acos(vectorReference.CosineAngle2D(playerToFocus)) : 360 - acos(vectorReference.CosineAngle2D(playerToFocus));

			if (FocusAngle < smallestAngle)
			{
				smallestAngle = FocusAngle;
				SetFocusNav(focusedActors[i]);
			}
		}
	}
}
void ACharacter_Player::SetFocusToPreviousFocus()
{
	FVector playerPos = GetActorLocation();
	FVector vectorReference = focus->GetActorLocation() - playerPos;
	float smallestAngle = 360;

	for (int i = 0; i < focusedActors.Num(); ++i)
	{
		if (focusedActors[i] != focus)
		{
			FVector playerToFocus = focusedActors[i]->GetActorLocation() - playerPos;
			float FocusAngle = (FVector::CrossProduct(vectorReference, playerToFocus).Z < 0) ? acos(vectorReference.CosineAngle2D(playerToFocus)) : 360 - acos(vectorReference.CosineAngle2D(playerToFocus));

			if (FocusAngle < smallestAngle)
			{
				smallestAngle = FocusAngle;
				SetFocusNav(focusedActors[i]);
			}
		}
	}
}

void ACharacter_Player::LookAtFocus(bool lerp)
{
	if (focus)
	{
		FRotator temp = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), focus->GetActorLocation());
		temp.Pitch = 0;
		if (lerp)
			SetActorRotation(FMath::Lerp(GetActorRotation(), temp, rotationSpeedWhenChangeFocus));
		else
			SetActorRotation(temp);
	}
}

void ACharacter_Player::StopDashBackRecovery()
{
	isDashBackInCooldown = false;
}
void ACharacter_Player::StopDashHitRecovery()
{
	isDashHitInCooldown = false;
}