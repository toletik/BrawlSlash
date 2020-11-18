// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LDBrick_TriggerFight.generated.h"

UCLASS()
class BRAWLSLASH_API ALDBrick_TriggerFight : public AActor
{
    GENERATED_BODY()

        ALDBrick_TriggerFight();

    virtual void BeginPlay() override;

    UFUNCTION()
    void BoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* boxCollider;
    class ACharacter_Player* playerReference{ nullptr };

    UPROPERTY(EditAnywhere, Category = LDBrick, meta = (AllowPrivateAccess = "true"))
        class AMyAIDirector* enemyGroupToTrigger = nullptr;
};