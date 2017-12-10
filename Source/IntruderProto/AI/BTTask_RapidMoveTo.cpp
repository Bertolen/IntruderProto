// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_RapidMoveTo.h"

/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTTask_RapidMoveTo::UBTTask_RapidMoveTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Rapid Move To";
	AcceptableDistance = 100.0f;
}

EBTNodeResult::Type UBTTask_RapidMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!ControllerRef) {
		ControllerRef = Cast<AGuardController>(OwnerComp.GetAIOwner());
		
		if (!ControllerRef)
			return EBTNodeResult::Failed;
	}

	AActor* goal = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID()));

	EPathFollowingRequestResult::Type movementResult = ControllerRef->MoveToActor(goal, AcceptableDistance);

	return EBTNodeResult::Succeeded;
}