// Fill out your copyright notice in the Description page of Project Settings.


#include "LDBrick_ShieldPoints.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/Character_Player.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "UObject/ConstructorHelpers.h"

ALDBrick_ShieldPoints::ALDBrick_ShieldPoints()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	outerSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_OuterSphere"));
	outerSphere->SetStaticMesh(SphereMeshAsset.Object);
	SetRootComponent(outerSphere);

	innerSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_InnerSphere"));
	innerSphere->SetStaticMesh(SphereMeshAsset.Object);
	innerSphere->SetupAttachment(outerSphere);

	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	sphereCollider->SetupAttachment(outerSphere);

	shieldPoints = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldPoints"));
	shieldPoints->SetupAttachment(outerSphere);
}

void ALDBrick_ShieldPoints::BeginPlay()
{
	Super::BeginPlay();

	playerReference = Cast<ACharacter_Player>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ALDBrick_ShieldPoints::SphereBeginOverlap);

	initialPos = GetActorLocation();

	FOnTimelineFloat progressFunctionBegin;
	progressFunctionBegin.BindUFunction(this, FName("OnTimelineInitialHoverUpdate"));
	timelineInitialHover.AddInterpFloat(curveFloatInitialHover, progressFunctionBegin);
	timelineInitialHover.PlayFromStart();
	
	FOnTimelineFloat progressFunctionOnOverlap;
	FOnTimelineEvent endFunctionOnOverlap;
	progressFunctionOnOverlap.BindUFunction(this, FName("OnTimelineShapeUpUpdate"));
	endFunctionOnOverlap.BindUFunction(this, FName("OnTimelineShapeUpEnd"));
	timelineShapeUp.AddInterpFloat(curveFloatShapeUp, progressFunctionOnOverlap);
	timelineShapeUp.SetTimelineFinishedFunc(endFunctionOnOverlap);
}

// Called every frame
void ALDBrick_ShieldPoints::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timelineInitialHover.TickTimeline(DeltaTime);
	timelineShapeUp.TickTimeline(DeltaTime);
}

void ALDBrick_ShieldPoints::SphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp == playerReference->GetCapsuleComponent())
	{
		sphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		playerReference->currentShieldPoint = playerReference->shieldPointMax;
		playerReference->PlayerGetShieldPoint();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), shieldPointSound, GetActorLocation());
		timelineShapeUp.PlayFromStart();
	}
}

void ALDBrick_ShieldPoints::OnTimelineInitialHoverUpdate()
{
	float timelineValue = timelineInitialHover.GetPlaybackPosition();
	SetActorLocation(GetActorLocation() + FVector(0, 0, curveFloatInitialHover->GetFloatValue(timelineValue)) );	

}

void ALDBrick_ShieldPoints::OnTimelineShapeUpUpdate()
{
	float timelineValue = timelineShapeUp.GetPlaybackPosition();
	outerSphere->SetRelativeScale3D(FVector(curveFloatShapeUp->GetFloatValue(timelineValue)));
}
void ALDBrick_ShieldPoints::OnTimelineShapeUpEnd()
{
	Destroy();
}