// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardController.h"
// Ai Components
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AGuardController::AGuardController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TargetLocationKeyName = FName("TargetLocation");
	TargetToFollowKeyName = FName("TargetToFollow");
}

// Called when the game starts or when spawned
void AGuardController::BeginPlay()
{
	Super::BeginPlay();

	UseBlackboard(BlackboardAsset, BlackboardComp);

	BlackboardComp->SetValueAsVector(FName("HomeLocation"), GetPawn()->GetActorLocation());

	RunBehaviorTree(BTAsset);
}

void AGuardController::SetTargetToFollow(UObject* target)
{
	if (BlackboardComp) {
		BlackboardComp->SetValueAsObject(TargetToFollowKeyName, target);
	}
}

void AGuardController::SetTargetLocation(FVector location)
{
	if (BlackboardComp) {
		BlackboardComp->SetValueAsVector(TargetLocationKeyName, location);
	}
}