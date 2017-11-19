// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AGuardController::AGuardController(const FObjectInitializer& ObjectInitializer)
	:Super()
{
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSense"));
	if (Sight) {
		Sight->DetectionByAffiliation.bDetectNeutrals = true;
		Sight->PeripheralVisionAngleDegrees = 45.0f;
	}

	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
	
	if (GetPerceptionComponent()) {
		GetPerceptionComponent()->ConfigureSense(*Sight);
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AGuardController::OnPerceptionUpdated);
	}
}

void AGuardController::OnPerceptionUpdated(TArray<AActor*> Actors)
{
	UE_LOG(LogClass, Log, TEXT("Found %i actor(s)"), Actors.Num());
	for (int i = 0 ; i < Actors.Num() ; i++)
	{
		UE_LOG(LogClass, Log, TEXT("   %s"), *Actors[i]->GetFName().ToString());
	}
}