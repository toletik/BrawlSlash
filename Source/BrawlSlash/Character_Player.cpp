// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Player.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"

#include "DrawDebugHelpers.h"

// Sets default values
ACharacter_Player::ACharacter_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set our turn rates for input
	baseTurnRate = 45.f;
	baseLookUpRate = 45.f;

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
	cameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	cameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	followCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacter_Player::Attack);
	PlayerInputComponent->BindAction("Counter", IE_Pressed, this, &ACharacter_Player::Counter);
	PlayerInputComponent->BindAction("Execution", IE_Pressed, this, &ACharacter_Player::Execution);

	
}

// Called when the game starts or when spawned
void ACharacter_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacter_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//GetInputAxisValue("MoveForward");
	//GetInputAxisValue("MoveRight");

	
	//check if something in the way
	FHitResult hit;
	FCollisionQueryParams raycastParams;
	raycastParams.AddIgnoredActor(this);
	FVector direction {GetInputAxisValue("MoveForward"), GetInputAxisValue("MoveRight"), 0};
	GetWorld()->LineTraceSingleByChannel(hit, GetActorLocation(), GetActorLocation() + direction * 800, ECC_Pawn, raycastParams);
	
	if (hit.GetActor() != nullptr && hit.GetActor()->ActorHasTag("highlightable"))
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + direction * 800, FColor::Green, false, 0.05, 0, 5);

		if (Cast<ACharacter_Base>(hit.GetActor()) != NULL)
		{
			if(elementToHighlight != nullptr)
				elementToHighlight->matDynamic->SetScalarParameterValue("needToGlow", 0);

			elementToHighlight = Cast<ACharacter_Base>(hit.GetActor());
			elementToHighlight->matDynamic->SetScalarParameterValue("needToGlow", 1);
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + direction * 800, FColor::Red, false, 0.05, 0, 5);

		if (elementToHighlight != nullptr)
			elementToHighlight->matDynamic->SetScalarParameterValue("needToGlow", 0);

		elementToHighlight = nullptr;
	}


}


//Left Joystick
void ACharacter_Player::MoveForward(float Value)
{
	if (!isAttacking && (Controller != NULL) && (Value != 0.0f))
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
	if (!isAttacking && (Controller != NULL) && (Value != 0.0f))
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
	AddControllerYawInput(Rate * baseTurnRate * GetWorld()->GetDeltaSeconds());
}
void ACharacter_Player::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//Buttons
void ACharacter_Player::Counter()
{
	isCountering = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Counter");
}

void ACharacter_Player::Execution()
{
	isExecuting = true;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Execution");
}

