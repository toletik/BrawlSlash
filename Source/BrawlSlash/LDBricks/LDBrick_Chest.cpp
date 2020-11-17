// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_Chest.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"

#include "LDBrick_ShieldPoints.h"

ALDBrick_Chest::ALDBrick_Chest()
{
	PrimaryActorTick.bCanEverTick = true;

	root = CreateDefaultSubobject<USceneComponent>(TEXT("ChestRoot"));
	SetRootComponent(root);

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_Chest"));
	mesh->SetupAttachment(RootComponent);

	particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PE_Chest"));
	particles->SetupAttachment(RootComponent);

	audio = CreateDefaultSubobject<UAudioComponent>(TEXT("SE_Chest"));
	audio->SetupAttachment(RootComponent);
}

void ALDBrick_Chest::BeginPlay()
{
	Super::BeginPlay();

	SetRootComponent(audio);

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	initalMeshScale = mesh->GetComponentScale();
	health = maxHealth;

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("OnTimelineUpdate"));
	timelineShapeUp.AddInterpFloat(curveFloat, ProgressFunction);
}

// Called every frame
void ALDBrick_Chest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timelineShapeUp.TickTimeline(DeltaTime);
}



void ALDBrick_Chest::TakeHit(int damage)
{
	health -= damage;
	audio->Stop();
	audio->Play();

	//on hit
	if (health > 0)
		timelineShapeUp.PlayFromStart();
	//on break
	else
		GetWorldTimerManager().SetTimer(timerAfterBreak, this, &ALDBrick_Chest::Break, 0.2, false);
}

void ALDBrick_Chest::OnTimelineUpdate(float value)
{
	float timelineValue = timelineShapeUp.GetPlaybackPosition();
	mesh->SetWorldScale3D(initalMeshScale * curveFloat->GetFloatValue(timelineValue));
}

void ALDBrick_Chest::Break()
{
	particles->Activate();

	FActorSpawnParameters spawnParams;
	GetWorld()->SpawnActor<AActor>(shieldPointClassType, GetActorLocation() + FVector(0, 0, 70), FRotator::ZeroRotator, spawnParams);

	mesh->DestroyComponent();
}