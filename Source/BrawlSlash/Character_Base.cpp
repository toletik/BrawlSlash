// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Base.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/BoxComponent.h"

// Sets default values
ACharacter_Base::ACharacter_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	health = maxHealth;

	attackBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBox"));
	attackBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACharacter_Base::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	USkeletalMeshComponent* mesh = GetMesh();
	UMaterialInterface* mat = mesh->GetMaterial(0);
	matDynamic = mesh->CreateDynamicMaterialInstance(0, mat);

	attackBox->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Base::AttackOverlap);
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called every frame
void ACharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//need to be put in takeDamage
	if (health <= 0)
		state = E_STATE::DEAD;
}

void ACharacter_Base::BeginAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACharacter_Base::EndAttack()
{
	attackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACharacter_Base::AttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor->ActorHasTag("Player") && ActorHasTag("Enemy"))
	||  (OtherActor->ActorHasTag("Enemy") && ActorHasTag("Player")))
		Cast<ACharacter_Base>(OtherActor)->TakeDamage(1);
}

void ACharacter_Base::TakeDamage(int damage)
{
	health -= damage;

	if (health > 0)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, GetName().Append(" : ").Append(FString::FromInt(health)));
	else
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, GetName().Append(" : ").Append(FString::FromInt(health)));


}
