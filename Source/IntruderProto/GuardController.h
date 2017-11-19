// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardController.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API AGuardController : public AAIController
{
	GENERATED_BODY()

public:
	// class constructor
	AGuardController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	UFUNCTION()
	virtual void OnPerceptionUpdated(TArray<AActor*> Actors);

private:
	UPROPERTY()
	class UAISenseConfig_Sight* Sight;
};
