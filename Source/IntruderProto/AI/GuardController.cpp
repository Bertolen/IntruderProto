// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

// Called when the game starts or when spawned
void AGuardController::BeginPlay()
{
	Super::BeginPlay();

	UseBlackboard(BlackboardAsset, BlackboardComp);

	BlackboardComp->SetValueAsVector(FName("HomeLocation"), GetPawn()->GetActorLocation());

	RunBehaviorTree(BTAsset);
}