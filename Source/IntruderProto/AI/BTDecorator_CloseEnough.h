// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CloseEnough.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API UBTDecorator_CloseEnough : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

	UBTDecorator_CloseEnough(const FObjectInitializer& ObjectInitializer);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AcceptableDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetToFollow;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const override;
};
