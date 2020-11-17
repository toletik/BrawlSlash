// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_Door.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"


ALDBrick_Door::ALDBrick_Door()
{
	PrimaryActorTick.bCanEverTick = true;

	meshPoles = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_DoorPoles"));
	SetRootComponent(meshPoles);

	meshBoards = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_DoorBoards"));
	meshBoards->SetupAttachment(RootComponent);

	box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	box->SetupAttachment(meshBoards);

	particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PE_Door"));
	particles->SetupAttachment(RootComponent);

	audio = CreateDefaultSubobject<UAudioComponent>(TEXT("SE_Door"));
	audio->SetupAttachment(RootComponent);
}

void ALDBrick_Door::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	initalMeshScale = meshBoards->GetComponentScale();
	health = maxHealth;

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("OnTimelineUpdate"));
	timelineShapeUp.AddInterpFloat(curveFloat, ProgressFunction);
}

// Called every frame
void ALDBrick_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timelineShapeUp.TickTimeline(DeltaTime);
}

void ALDBrick_Door::TakeHit(int damage)
{
	health -= damage;
	audio->Stop();
	audio->Play();

	//on hit
	if (health > 0)
		timelineShapeUp.PlayFromStart();
	//on break
	else
		GetWorldTimerManager().SetTimer(timerAfterBreak, this, &ALDBrick_Door::Break, 0.2, false);
}

void ALDBrick_Door::OnTimelineUpdate()
{
	float timelineValue = timelineShapeUp.GetPlaybackPosition();
	meshBoards->SetWorldScale3D( initalMeshScale * curveFloat->GetFloatValue(timelineValue) );
}

void ALDBrick_Door::Break()
{
	particles->Activate();

	meshBoards->DestroyComponent();
	box->DestroyComponent();

	Tags.Remove("Focusable");
}