// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Characters/GuardCharacter.h"
#include "BTTask_FindNextWaypoint.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API UBTTask_FindNextWaypoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_FindNextWaypoint(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	AGuardCharacter* CharacterRef;

	UPROPERTY()
	class ATargetPoint* CurentWaypoint;
};
