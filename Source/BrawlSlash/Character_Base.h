// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Character_Base.generated.h"

UCLASS()
class BRAWLSLASH_API ACharacter_Base : public ACharacter
{
	GENERATED_BODY()

	float health = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics", meta = (AllowPrivateAccess = "true"))
	float maxHealth = 5.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isAttacking = false;



public:
	// Sets default values for this character's properties
	ACharacter_Base();

	UMaterialInstanceDynamic* matDynamic = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void Attack();

};
