// Fill out your copyright notice in the Description page of Project Settings.


#include "Task_LookAtPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "../Characters/Character_EnemyBase.h"
#include "AIController.h"

EBTNodeResult::Type UTask_LookAtPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter_EnemyBase* enemy = Cast<ACharacter_EnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (!enemy->beingDashedBack)
	{
		FVector selfToPlayer = OwnerComp.GetBlackboardComponent()->GetValueAsVector("playerPos") - enemy->GetActorLocation();

		//Setup the lookAt and nullifie the pitch
		FRotator lookAt = FRotationMatrix::MakeFromX(selfToPlayer).Rotator();
		lookAt.Pitch = 0;
		selfToPlayer.Normalize();

		if (enemy->rotateDashedBack)
		{
			if (FVector::DotProduct(enemy->GetActorForwardVector(), selfToPlayer) > 0.99f)
				enemy->rotateDashedBack = false;

			enemy->SetActorRotation(FMath::Lerp(enemy->GetActorRotation(), lookAt, enemy->rotateSpeedWhenBeingDashedBack));
		}

		else
			enemy->SetActorRotation(lookAt);
	}
	return EBTNodeResult::Succeeded;
}