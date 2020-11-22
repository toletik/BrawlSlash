// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_DashPoint.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"

#include "UObject/ConstructorHelpers.h"


ALDBrick_DashPoint::ALDBrick_DashPoint()
{ 	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	outerSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_OuterSphere"));
	outerSphere->SetStaticMesh(SphereMeshAsset.Object);
	SetRootComponent(outerSphere);

	innerSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_InnerSphere"));
	innerSphere->SetStaticMesh(SphereMeshAsset.Object);
	innerSphere->SetupAttachment(outerSphere);

	waitForRepopSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_WaitForRepopSphere"));
	waitForRepopSphere->SetStaticMesh(SphereMeshAsset.Object);
	waitForRepopSphere->SetupAttachment(outerSphere);
}

void ALDBrick_DashPoint::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ALDBrick_DashPoint::Tick(float DeltaTime)
{
	//make cos in range [-0.25, 0.25]
	outerSphere->SetWorldScale3D(FVector(1.5 + cos(GetWorld()->TimeSeconds) / 4 ));
}


void ALDBrick_DashPoint::OnPlayerEndDash()
{
	playerReference->LaunchCharacter(playerReference->GetActorRotation().RotateVector(launchDirection * launchForce), true, true);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), glassBrokenSystem, GetActorLocation(), { 0, 0, 0 }, { 1, 1, 1 }, false, true);
	UGameplayStatics::PlaySound2D(GetWorld(), glassBrokenSound);

	outerSphere->SetVisibility(false);
	innerSphere->SetVisibility(false);
	waitForRepopSphere->SetVisibility(true);
	innerSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(timerForRespawn, this, &ALDBrick_DashPoint::OnRepop, timeBeforeRepop, false);
}

void ALDBrick_DashPoint::OnRepop()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DashPointSpawnSound, GetActorLocation());
	outerSphere->SetVisibility(true);
	innerSphere->SetVisibility(true);
	waitForRepopSphere->SetVisibility(false);
	innerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
