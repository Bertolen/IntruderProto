// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardController.h"
#include "AI/GuardCharacter.h"

/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AGuardController::AGuardController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}

void AGuardController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	AGuardCharacter* character = Cast<AGuardCharacter>(InPawn);
	if (character)
	{
		if (character->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*character->BehaviorTree->BlackboardAsset);

			/* Make sure the Blackboard has the type of bot we possessed */
			//SetBlackboardBotType(character->BotType);
		}

		BehaviorComp->StartTree(*character->BehaviorTree);
	}
}

void AGuardController::UnPossess()
{
	Super::UnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
}
void AGuardController::SetTargetEnemy(APawn* NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}
/*
void AGuardController::SetBlackboardBotType(EBotBehaviorType NewType)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(BotTypeKeyName, (uint8)NewType);
	}
}
*/