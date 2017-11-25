// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_CloseEnough.h"
#include "IntruderProto/AI/GuardController.h"

/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTDecorator_CloseEnough::UBTDecorator_CloseEnough(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Close Enough";
	AcceptableDistance = 100.0f;
}

bool UBTDecorator_CloseEnough::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	AGuardController* controller = Cast<AGuardController>(OwnerComp.GetAIOwner());
	if (!controller) { // if there's no controller then we always send a negative result
		return IsInversed();
	}

	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(TargetToFollow.SelectedKeyName));
	if (!target) { // if there's no target then we always send a negative result
		return IsInversed();
	}

	FVector targetLocation = target->GetActorLocation();

	FVector ourLocation = controller->GetPawn()->GetActorLocation();

	float remainingDistance = (targetLocation - ourLocation).Size();

	return (remainingDistance >= AcceptableDistance);
}