// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindNextWaypoint.h"

/* AI Module includes */
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "Classes/AIController.h"

UBTTask_FindNextWaypoint::UBTTask_FindNextWaypoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Find Next Waypoint";
	CurentWaypoint = FVector::ZeroVector;
}

EBTNodeResult::Type UBTTask_FindNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Finds the character
	if (!CharacterRef) {
		CharacterRef = Cast<AGuardCharacter>(OwnerComp.GetAIOwner()->GetPawn());

		if (!CharacterRef) {
			return EBTNodeResult::Failed;
		}
	}

	// If there's no waypoints then we stop before crashing
	if (CharacterRef->GetWaypoints().Num() == 0) {
		return EBTNodeResult::Failed;
	}

	int index = 0;

	// if the current waypoint is the zero vector then we'll use the first index
	if (CurentWaypoint != FVector::ZeroVector) {
		
		if (!CharacterRef->GetWaypoints().Find(CurentWaypoint, index)){
			return EBTNodeResult::Failed;
		}

		index++;

		if (index >= CharacterRef->GetWaypoints().Num()) {
			index = 0;
		}
	}
	
	CurentWaypoint = CharacterRef->GetWaypoints()[index];
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, CurentWaypoint);
	
	return EBTNodeResult::Succeeded;
}