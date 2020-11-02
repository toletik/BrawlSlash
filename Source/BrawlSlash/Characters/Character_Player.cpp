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

	
	focusDetector = CreateDefaultSubobject<USphereComponent>(TEXT("focusDetector"));
	focusDetector->SetupAttachment(RootComponent);
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

	PlayerInputComponent->BindAxis("TurnRateFixed", this, &ACharacter_Player::TurnAtRateFixed);

	//Buttons
	PlayerInputComponent->BindAction("DashBack", IE_Pressed, this, &ACharacter_Player::StartDashBack);
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

	Controller->SetControlRotation(fixedRotation);
	cameraBoom->CameraLagMaxDistance = positionLerpLimitRange;
	SetCameraStatsNav();

	APlayerCameraManager* const camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	camManager->ViewPitchMin = -verticalAngleMax;
	camManager->ViewPitchMax = -verticalAngleMin;
	
	attackBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Player::AttackOverlap);

	//Focus Detector
	focusDetector->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Player::FocusDetectorBeginOverlap);
	focusDetector->OnComponentEndOverlap.AddDynamic(this, &ACharacter_Player::FocusDetectorEndOverlap);
}

// Called every frame
void ACharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTimers();

	UpdateDebug();

	UpdateCamera();

	//Look at focus while idle
	if (GetVelocity().Size() < 0.5f)
		LookAtFocus(true);

	UpdatePosToStickPoint();

	UpdateDashingHit();

	UpdateDashingBack();
}

void ACharacter_Player::UpdateTimers()
{
	if (currentInvincibleTime >= 0)
	{
		currentInvincibleTime -= GetWorld()->GetDeltaSeconds();

		if (currentInvincibleTime < 0)
			PlayerEndInvincibleTime();
	}

	currentTimeForComeBack += GetWorld()->GetDeltaSeconds();
}
void ACharacter_Player::UpdateDebug()
{
	if (focus && (focus->GetActorLocation() - GetActorLocation()).Size() > minDistanceToDash && (focus->GetActorLocation() - GetActorLocation()).Size() < maxDistanceToDash)
		isFocusInShortRange = true;
	else
		isFocusInShortRange = false;

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
	if (currentEnemyGroup)
	{
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), rotationForFight, GetWorld()->GetDeltaSeconds(), 2));
		if (!currentEnemyGroup->IsAllEnemiesInCameraSight(GetWorld()->GetFirstPlayerController()))
			cameraBoom->TargetArmLength = FMath::Min(maxdistance, cameraBoom->TargetArmLength + zoomDezoomSpeed * GetWorld()->GetDeltaSeconds());
		else
			cameraBoom->TargetArmLength = FMath::Max(mindistance, cameraBoom->TargetArmLength - zoomDezoomSpeed * GetWorld()->GetDeltaSeconds());
	}
	else if (focus)
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), FRotationMatrix::MakeFromX(focus->GetActorLocation() - GetActorLocation() - GetActorUpVector() * 500).Rotator(), GetWorld()->GetDeltaSeconds(), 2));
	else if (currentTimeForComeBack > timeForComeBack)
	{
		fixedRotation = FRotationMatrix::MakeFromX(GetActorForwardVector().RotateAngleAxis(behindAngle, GetActorRightVector())).Rotator();
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), fixedRotation, GetWorld()->GetDeltaSeconds() * scaleRotationSpeedToBehind, 2));
	}
	else
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), fixedRotation, GetWorld()->GetDeltaSeconds(), 2));
}
void ACharacter_Player::UpdatePosToStickPoint()
{
	if (isGoingToStickPoint && focus)
	{
		FVector direction = focus->GetActorLocation() - GetActorLocation();

		if (direction.Size() < stickRange && direction.Size() > stickPoint && state == E_STATE::ATTACKING)
			AddMovementInput(direction, 1.0f);
		else
			isGoingToStickPoint = false;
	}
}
void ACharacter_Player::UpdateDashingHit()
{
	if (state == E_STATE::DASHING_HIT)
	{
		FHitResult hit;
		FCollisionQueryParams raycastParams;
		raycastParams.AddIgnoredActor(this);
		FVector direction = focus->GetActorLocation() - GetActorLocation();
		direction.Normalize();
		if (currentEnemyGroup)
			direction *= stickPoint;
		GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + direction, ECC_WorldDynamic, raycastParams);

		if (hit.GetActor() && hit.GetActor() == focus)
			StopDashHit();
	}
}
void ACharacter_Player::UpdateDashingBack()
{
	if (focus)
		GEngine->AddOnScreenDebugMessage(-96, 0.4f, FColor::Cyan, FString::FromInt(FVector::DotProduct(testForDashBack, focus->GetActorLocation() - GetActorLocation()) ) );
	
	if (state == E_STATE::DASHING_BACK &&
		FVector::DotProduct(testForDashBack, focus->GetActorLocation() - GetActorLocation() ) < 0 &&
		(focus->GetActorLocation() - GetActorLocation()).Size() > stickPoint)
	{
		
		SetActorLocation(focus->GetActorLocation() + GetActorForwardVector() * stickPoint);
		LookAtFocus(false);
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		SetActorEnableCollision(true);
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		if (currentEnemyGroup)
			currentEnemyGroup->UpdateIfIsInInner();
		state = E_STATE::IDLE;
		ACharacter_EnemyBase* enemyFocus = Cast<ACharacter_EnemyBase>(focus);
		if (enemyFocus)
		{
			enemyFocus->notLookAtPlayer = true;
			GetWorldTimerManager().SetTimer(enemyFocus->timerHandler, enemyFocus, &ACharacter_EnemyBase::LookAtPlayer, enemyFocus->timeBeforeRotateWhenBeingDashedBack, false);
		}
	}
}

void ACharacter_Player::FocusDetectorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Focusable"))
	{
		if (!currentEnemyGroup)
			SetFocusNav(OtherActor);
		focusedActors.Add(OtherActor);
	}
}

void ACharacter_Player::FocusDetectorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Focusable"))
	{
		focusedActors.Remove(OtherActor);
		if (!currentEnemyGroup && focus == OtherActor)
			SetFocusToClosestFocus();
	}
}

void ACharacter_Player::AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter_EnemyBase* enemyCast = Cast<ACharacter_EnemyBase>(OtherActor);

	if (enemyCast)
	{
		if (!enemyCast->ShieldCheckProtection(GetActorLocation()))
			enemyCast->TakeHit(toDoDamage, state);
		else
		{
			enemyCast->ShieldHitted();
			state = E_STATE::PUSHED_BACK;
			LaunchCharacter(-GetActorForwardVector() * knockbackForceAfterAttackBlocked, true, true);
		}
		toDoDamage = 0;
	}
}

void ACharacter_Player::TakeHit(int damage, E_STATE attackerState)
{
	if (currentInvincibleTime >= 0)
		return;

	Super::TakeHit(damage, attackerState);

	if (attackerState == E_STATE::ATTACKING_WEAK)
		state = E_STATE::HITTED_WEAK;

	else if (attackerState == E_STATE::ATTACKING_STRONG)
		state = E_STATE::HITTED_STRONG;

	currentInvincibleTime = invincibleTime;
	PlayerStartInvincibleTime();
	PlayerStartHitted();
}

//Left Joystick
void ACharacter_Player::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (state == E_STATE::ATTACKING && actualCombo == 3 && canCancelCombo)
		{
			state = E_STATE::IDLE;
			actualCombo = 0;
			canCancelCombo = false;
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
	}
}
void ACharacter_Player::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		if (state == E_STATE::ATTACKING && actualCombo == 3 && canCancelCombo)
		{
			state = E_STATE::IDLE;
			actualCombo = 0;
			canCancelCombo = false;
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
		currentTimeForComeBack = 0.0f;
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * rotationSpeedHorizontal * GetWorld()->GetDeltaSeconds() * (gameInstance->isXRevert ? -1.0f : 1.0f));
}
void ACharacter_Player::LookUpAtRate(float Rate)
{
	if (Rate != 0.0f)
		currentTimeForComeBack = 0.0f;
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * rotationSpeedVertical * GetWorld()->GetDeltaSeconds() * (gameInstance->isYRevert ? -1.0f : 1.0f));
}
void ACharacter_Player::TurnAtRateFixed(float Rate)
{
	if (Rate != 0.0f)
		currentTimeForComeBack = 0.0f;

	if (!currentEnemyGroup && !focus)
		fixedRotation.Yaw += Rate * rotationSpeedHorizontalFixed * GetWorld()->GetDeltaSeconds() * (gameInstance->isXRevert ? -1.0f : 1.0f);
}

void ACharacter_Player::DashAttack()
{
	LookAtFocus(false);
	StopCombo();
	toDoDamage = dashHitDamage;
	state = E_STATE::ATTACKING;
}

//Buttons
void ACharacter_Player::Attack()
{
	if (state == E_STATE::DASHING_BACK || state == E_STATE::DASHING_HIT || state == E_STATE::PREPARING_DASH)
		return;

	if (state == E_STATE::ATTACKING && canCombo)
		needToAttack = true;
	else
	{
		if (focus && minDistanceToDash < (focus->GetActorLocation() - GetActorLocation()).Size() && (focus->GetActorLocation() - GetActorLocation()).Size() < maxDistanceToDash)
			StartDash(E_STATE::DASHING_HIT);
		else
		{
			LookAtFocus(false);

			state = E_STATE::ATTACKING;

			isGoingToStickPoint = true;

			if (needToAttack || canCombo)
			{
				GetWorldTimerManager().ClearTimer(timerHandler);
				canCombo = false;
				needToAttack = false;

				actualCombo++;

				if (actualCombo == 2)
					toDoDamage = secondComboDamage;

				else
					toDoDamage = thirdComboDamage;
			}

			else
			{
				actualCombo = 1;
				toDoDamage = firstComboDamage;
			}
		}
	}
}

void ACharacter_Player::StartDashBack()
{
	if (currentEnemyGroup)
		StartDash(E_STATE::DASHING_BACK);
}

void ACharacter_Player::StartDash(E_STATE teleportState)
{
	if (state == E_STATE::PREPARING_DASH || state == E_STATE::DASHING_BACK || state == E_STATE::DASHING_HIT)
		return;

	state = E_STATE::PREPARING_DASH;

	if (teleportState == E_STATE::DASHING_BACK)
		GetWorldTimerManager().SetTimer(timerHandler, this, &ACharacter_Player::DashBack, preparingDashDuration, false);

	else if (teleportState == E_STATE::DASHING_HIT)
		GetWorldTimerManager().SetTimer(timerHandler, this, &ACharacter_Player::DashHit, preparingDashDuration, false);
}

void ACharacter_Player::DashHit()
{
	state = E_STATE::DASHING_HIT;
	GetWorldTimerManager().ClearTimer(timerHandler);

	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	FVector direction = focus->GetActorLocation() - GetActorLocation();
	direction.Normalize();
	direction *= 10000.0f;

	if (currentEnemyGroup)
	{
		SetActorEnableCollision(false);
		if (FVector::DotProduct(-direction, focus->GetActorForwardVector()) < 0)
			direction -= focus->GetActorForwardVector() * stickPoint;
		else
			direction += focus->GetActorForwardVector() * stickPoint;
	}
	LaunchCharacter(direction, true, true);
}

void ACharacter_Player::DashBack()
{
	state = E_STATE::DASHING_BACK;
	GetWorldTimerManager().ClearTimer(timerHandler);

	GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
	FVector direction = focus->GetActorLocation() - GetActorLocation();
	direction.Normalize();
	if (FVector::DotProduct(-direction, focus->GetActorForwardVector()) < 0)
		direction = direction * 10000.0f + focus->GetActorForwardVector() * stickPoint;
	else
		direction = direction * 10000.0f - focus->GetActorForwardVector() * stickPoint;
	LaunchCharacter(direction, true, true);
	SetActorEnableCollision(false);

	testForDashBack = direction;
}

void ACharacter_Player::StopCombo()
{
	GetWorldTimerManager().ClearTimer(timerHandler);
	canCombo = false;
	actualCombo = 0;
	state = E_STATE::IDLE;
}

void ACharacter_Player::StopDashHit()
{
	GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	if (currentEnemyGroup)
	{
		SetActorEnableCollision(true);
		currentEnemyGroup->UpdateIfIsInInner();
		DashAttack();
	}
	else
	{
		state = E_STATE::IDLE;
		LaunchCharacter(FVector::UpVector * jumpForceAfterNavDashHit, true, true);
	}
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
	cameraBoom->TargetArmLength = distanceNav;
	cameraBoom->CameraLagSpeed = LerpSpeedNav;
	followCamera->SetFieldOfView(fovNav);

	cameraBoom->SetRelativeLocation({0, 0, cameraHeightNav });
}
void ACharacter_Player::SetCameraStatsLookAt()
{
	cameraBoom->TargetArmLength = distanceLookAt;
	cameraBoom->CameraLagSpeed = LerpSpeedLookAt;
	followCamera->SetFieldOfView(fovLookAt);

	cameraBoom->SetRelativeLocation({0, 0, cameraHeightLookAt});
}
void ACharacter_Player::SetCameraStatsFight(FRotator rotationToAdopt)
{
	cameraBoom->TargetArmLength = distanceFight;
	cameraBoom->CameraLagSpeed = LerpSpeedFight;
	followCamera->SetFieldOfView(fovFight);

	cameraBoom->SetRelativeLocation({ 0, 0, cameraHeightFight});
	rotationForFight = rotationToAdopt;
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