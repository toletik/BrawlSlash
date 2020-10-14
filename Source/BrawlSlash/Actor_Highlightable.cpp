// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values
AActor_Highlightable::AActor_Highlightable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActor_Highlightable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AActor_Highlightable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
