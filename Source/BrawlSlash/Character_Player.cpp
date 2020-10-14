// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Player.h"
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
	PlayerInputComponent->BindAction("Tp", IE_Pressed, this, &ACharacter_Player::StartTeleport);
	PlayerInputComponent->BindAction("Tp", IE_Released, this, &ACharacter_Player::StopTeleport);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacter_Player::Attack);
	PlayerInputComponent->BindAction("Execution", IE_Pressed, this, &ACharacter_Player::Execution);
}

// Called when the game starts or when spawned
void ACharacter_Player::BeginPlay()
{
	Super::BeginPlay();
	
	gameInstance = Cast<UMyGameInstance>(GetGameInstance());

	cameraBoom->TargetArmLength = distanceFromPlayer;
	Controller->SetControlRotation(initialRotation);
	cameraBoom->CameraLagSpeed = positionLerpSpeed;
	cameraBoom->CameraLagMaxDistance = positionLerpLimitRange;

	APlayerCameraManager* const camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	camManager->ViewPitchMin = -verticalAngleMax;
	camManager->ViewPitchMax = -verticalAngleMin;
	
	currentMobilityPoints = maxMobilityPoints;

	coneJoint = Cast<USceneComponent>(GetComponentsByTag(USceneComponent::StaticClass(), "Cone")[0]);
	coneMesh = Cast<UStaticMeshComponent>(coneJoint->GetChildComponent(0));
	UMaterialInterface* coneTempMat = coneMesh->GetMaterial(0);
	coneMat = coneMesh->CreateDynamicMaterialInstance(0, coneTempMat);

	coneMesh->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Player::ConeBeginOverlap);
	coneMesh->OnComponentEndOverlap.AddDynamic(this, &ACharacter_Player::ConeEndOverlap);
	coneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	coneMesh->SetVisibility(false);
}

// Called every frame
void ACharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	 //if (focus != nullptr)
	 //    Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), FRotationMatrix::MakeFromX(focus->GetActorLocation() - GetActorLocation() - GetActorUpVector() * 500 ).Rotator(), GetWorld()->GetDeltaSeconds(), 2));\

	//Look at focus while idle
	if (focus && GetVelocity().Size() < 0.5f)
	{
		FRotator temp = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), focus->GetActorLocation());
		temp.Pitch = 0;
		SetActorRotation(temp);
	}

	if (state == E_STATE::AIMING)
		UpdateTarget();

	if (state == E_STATE::DASHING && focus)
	{
		if ((focus->GetActorLocation() - GetActorLocation()).Size() < 100.0f)
		{
			Attack();
			GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
		}
	}

	if (isInFight)
		Controller->SetControlRotation(UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), (FVector::ForwardVector.RotateAngleAxis(fightAngle, FVector::RightVector)).ToOrientationRotator(), GetWorld()->GetDeltaSeconds(), 2));
}


//Left Joystick
void ACharacter_Player::MoveForward(float Value)
{
	if (state != E_STATE::ATTACKING && state != E_STATE::AIMING && state != E_STATE::DASHING && (Controller != NULL) && (Value != 0.0f))
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
	if (state != E_STATE::ATTACKING && state != E_STATE::AIMING && state != E_STATE::DASHING && (Controller != NULL) && (Value != 0.0f))
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
	if (state == E_STATE::AIMING)
		return;
	
	if (state == E_STATE::ATTACKING)
	{
		if(canCombo)
			needToAttack = true;
	}
	else
	{
		currentMobilityPoints = FMath::Min(currentMobilityPoints + onAttackMobilityPoints, maxMobilityPoints);
		state = E_STATE::ATTACKING;

		if (needToAttack || canCombo)
		{
			GetWorldTimerManager().ClearTimer(timerHandler);
			canCombo = false;
			needToAttack = false;

			actualCombo++;


			//make an array of combodamage and [actualcombo]
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

void ACharacter_Player::TakeHit(int damage)
{
	Super::TakeHit(damage);

	state = E_STATE::HITTED_WEAK;
}

void ACharacter_Player::Execution()
{
	state = E_STATE::EXECUTING;
}

void ACharacter_Player::StartAiming()
{
	state = E_STATE::AIMING;
	coneMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	coneMesh->SetVisibility(true);
}

void ACharacter_Player::StartTeleport()
{
	GetWorldTimerManager().SetTimer(timerHandler, this, &ACharacter_Player::StartAiming, timeToStartAiming, false);
}

void ACharacter_Player::StopTeleport()
{	
	if (state == E_STATE::AIMING)
	{
		if (target && currentMobilityPoints - onTpHitMobilityPoints >= 0)
		{
			currentMobilityPoints -= onTpHitMobilityPoints;

			state = E_STATE::DASHING;
			focus = Cast<AActor>(target);
			ChangeFocus();
			GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
			LaunchCharacter((focus->GetActorLocation() - GetActorLocation()) * 10.0f, true, true);
		}

		else
			state = E_STATE::IDLE;
	}

	else
	{
		GetWorldTimerManager().ClearTimer(timerHandler);

		if (focus && currentMobilityPoints - onDodgeMobilityPoints >= 0)
		{
			SetActorLocation(focus->GetActorLocation() - focus->GetActorForwardVector() * 200.0f);
			currentMobilityPoints -= onDodgeMobilityPoints;
		}
	}

	coneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	coneMesh->SetVisibility(false);
	target = nullptr;
}

void ACharacter_Player::StopCombo()
{
	GetWorldTimerManager().ClearTimer(timerHandler);
	canCombo = false;
	actualCombo = 0;
	state = E_STATE::IDLE;
}

void ACharacter_Player::UpdateTarget()
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	FVector direction = YawRotation.RotateVector(GetInputAxisValue("MoveForward") * FVector::ForwardVector + GetInputAxisValue("MoveRight") * FVector::RightVector);

	if (coneJoint)
		coneJoint->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, direction));

	if (target)
	{
		if (coneMat)
			coneMat->SetScalarParameterValue("Green", 1);
	}
	else
	{
		if (coneMat)
			coneMat->SetScalarParameterValue("Green", 0);
	}
}

void ACharacter_Player::ConeBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Target"))
	{
		overlappedTargets.Add(OtherActor);
		target = OtherActor;
	}
}
void ACharacter_Player::ConeEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Target"))
	{
		overlappedTargets.Remove(OtherActor);
		
		if (target && target == OtherActor)
			target = nullptr;
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
