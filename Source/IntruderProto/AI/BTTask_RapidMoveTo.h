// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "IntruderProto/AI/GuardController.h"
#include "BTTask_RapidMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API UBTTask_RapidMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_RapidMoveTo(const FObjectInitializer& ObjectInitializer);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY()
	AGuardController* ControllerRef;
};
