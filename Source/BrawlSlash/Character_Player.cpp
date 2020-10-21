// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Player.h"
#include "MyAIDirector.h"
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
	PlayerInputComponent->BindAction("Tp", IE_Pressed, this, &ACharacter_Player::StartBypass);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacter_Player::Attack);
	PlayerInputComponent->BindAction("FocusNextEnemy", IE_Pressed, this, &ACharacter_Player::GetNextFocus);
	PlayerInputComponent->BindAction("FocusPreviousEnemy", IE_Pressed, this, &ACharacter_Player::GetPreviousFocus);


}

// Called when the game starts or when spawned
void ACharacter_Player::BeginPlay()
{
	Super::BeginPlay();
	
	gameInstance = Cast<UMyGameInstance>(GetGameInstance());

	Controller->SetControlRotation(initialRotation);
	cameraBoom->CameraLagMaxDistance = positionLerpLimitRange;
	SetCameraStatsNav();

	APlayerCameraManager* const camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	camManager->ViewPitchMin = -verticalAngleMax;
	camManager->ViewPitchMax = -verticalAngleMin;
	
	currentMobilityPoints = maxMobilityPoints;

	attackBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Player::AttackOverlap);
}

// Called every frame
void ACharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//camera	
	if (isInFight)
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), (FVector::ForwardVector.RotateAngleAxis(fightAngle, FVector::RightVector)).ToOrientationRotator(), GetWorld()->GetDeltaSeconds(), 2));
	else if (focus)
			Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), FRotationMatrix::MakeFromX(focus->GetActorLocation() - GetActorLocation() - GetActorUpVector() * 500 ).Rotator(), GetWorld()->GetDeltaSeconds(), 2));
			
	//Look at focus while idle
	if (focus && GetVelocity().Size() < 0.5f)
	{
		FRotator temp = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), focus->GetActorLocation());
		temp.Pitch = 0;
		SetActorRotation(temp);
	}

	if (isGoingToStickPoint && focus && (focus->GetActorLocation() - GetActorLocation()).Size() - stickPoint > 100.0f)
	{
		isGoingToStickPoint = false;
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	}

	//clean one day
	if (state == E_STATE::DASHING && focus && (focus->GetActorLocation() - GetActorLocation()).Size() < stickPoint)
	{
		Attack();
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		SetActorEnableCollision(true);
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}

	if (state == E_STATE::BYPASSING && focus && (focus->GetActorLocation() - focus->GetActorForwardVector() * stickPoint - GetActorLocation()).Size() < stickPoint)
	{
		state = E_STATE::IDLE;
		GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		SetActorEnableCollision(true);
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
	}
}

void ACharacter_Player::AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter_EnemyBase* enemyCast = Cast<ACharacter_EnemyBase>(OtherActor);
	
	if (enemyCast)
	{
		enemyCast->TakeHit(toDoDamage, state);
		toDoDamage = 0;

		currentMobilityPoints = FMath::Min(currentMobilityPoints + onAttackMobilityPoints, maxMobilityPoints);
	}
}

//Left Joystick
void ACharacter_Player::MoveForward(float Value)
{
	if (state != E_STATE::ATTACKING && state != E_STATE::PREPARINGTELEPORT && state != E_STATE::DASHING && state != E_STATE::BYPASSING && (Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void ACharacter_Player::MoveRight(float Value)
{
	if (state != E_STATE::ATTACKING && state != E_STATE::PREPARINGTELEPORT && state != E_STATE::DASHING && state != E_STATE::BYPASSING && (Controller != NULL) && (Value != 0.0f))
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

//RightJoystick
void ACharacter_Player::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * rotationSpeedHorizontal * GetWorld()->GetDeltaSeconds() * (gameInstance->isXRevert? -1.0f : 1.0f) );
}
void ACharacter_Player::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * rotationSpeedVertical * GetWorld()->GetDeltaSeconds() * (gameInstance->isYRevert? -1.0f : 1.0f) );
}

//Buttons
void ACharacter_Player::Attack()
{
	if (state == E_STATE::ATTACKING)
	{
		if(canCombo)
			needToAttack = true;
	}
	else
	{
		if (state == E_STATE::DASHING)
		{
			StopCombo();
			toDoDamage = dashHitDamage;
			state = E_STATE::ATTACKING;
		}

		else if (focus && (focus->GetActorLocation() - GetActorLocation()).Size() > minDistanceToDash && (focus->GetActorLocation() - GetActorLocation()).Size() < maxDistanceToDash)
			StartTeleport(E_STATE::DASHING);

		else
		{
			state = E_STATE::ATTACKING;

			if (focus)
			{
				FVector direction = focus->GetActorLocation() - GetActorLocation();
				direction.Normalize();
				direction *= stickPoint * 10.0f;
				GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
				LaunchCharacter(direction, true, true);
				isGoingToStickPoint = true;
			}

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

void ACharacter_Player::StartBypass()
{
	StartTeleport(E_STATE::BYPASSING);
}

void ACharacter_Player::StartTeleport(E_STATE teleportState)
{
	if (!focus || state == E_STATE::PREPARINGTELEPORT || state == E_STATE::BYPASSING || state == E_STATE::DASHING || state == E_STATE::ATTACKING)
		return;

	state = E_STATE::PREPARINGTELEPORT;

	if (teleportState == E_STATE::BYPASSING)
		GetWorldTimerManager().SetTimer(timerHandler, this, &ACharacter_Player::Bypass, preparingDashDuration, false);

	if (teleportState == E_STATE::DASHING)
		GetWorldTimerManager().SetTimer(timerHandler, this, &ACharacter_Player::DashHit, preparingDashDuration, false);
}

void ACharacter_Player::DashHit()
{
	GetWorldTimerManager().ClearTimer(timerHandler);

	if (focus && currentMobilityPoints - onDashHitMobilityPoints >= 0)
	{
		currentMobilityPoints -= onDashHitMobilityPoints;

		state = E_STATE::DASHING;
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		LaunchCharacter((focus->GetActorLocation() - GetActorLocation()) * 10.0f, true, true);
		SetActorEnableCollision(false);
	}

	else
		state = E_STATE::IDLE;
}

void ACharacter_Player::Bypass()
{
	GetWorldTimerManager().ClearTimer(timerHandler);

	if (focus && currentMobilityPoints - onDashBackMobilityPoints >= 0)
	{
		state = E_STATE::BYPASSING;
		GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		LaunchCharacter((focus->GetActorLocation() - GetActorLocation()) * 10.0f, true, true);
		SetActorEnableCollision(false);
		currentMobilityPoints -= onDashBackMobilityPoints;
	}

	else
		state = E_STATE::IDLE;
}


void ACharacter_Player::StopCombo()
{
	GetWorldTimerManager().ClearTimer(timerHandler);
	canCombo = false;
	actualCombo = 0;
	state = E_STATE::IDLE;
}

void ACharacter_Player::SetFocusNav(AActor* newFocus)
{
	focus = newFocus;

	if (!isInFight)
	{
		if (focus)
			SetCameraStatsLookAt();
		else
			SetCameraStatsNav();
	}
}

void ACharacter_Player::TestRandomStart()
{
	GEngine->AddOnScreenDebugMessage(-17, 1.0f, FColor::Cyan, "Start");

}
void ACharacter_Player::TestRandomEnd()
{
	GEngine->AddOnScreenDebugMessage(-17, 1.0f, FColor::Cyan, "End");
}

void ACharacter_Player::GetNextFocus()
{
	if (currentEnemyGroup)
		currentEnemyGroup->SetFocusToNextEnemy();
}
void ACharacter_Player::GetPreviousFocus()
{
	if (currentEnemyGroup)
		currentEnemyGroup->SetFocusToPreviousEnemy();
}

void ACharacter_Player::SetCameraStatsNav()
{
	cameraBoom->TargetArmLength = distanceNav;
	cameraBoom->CameraLagSpeed = LerpSpeedNav;
	followCamera->SetFieldOfView(fovNav);
}
void ACharacter_Player::SetCameraStatsLookAt()
{
	cameraBoom->TargetArmLength = distanceLookAt;
	cameraBoom->CameraLagSpeed = LerpSpeedLookAt;
	followCamera->SetFieldOfView(fovLookAt);
}
void ACharacter_Player::SetCameraStatsFight()
{
	cameraBoom->TargetArmLength = distanceFight;
	cameraBoom->CameraLagSpeed = LerpSpeedFight;
	followCamera->SetFieldOfView(fovFight);
}