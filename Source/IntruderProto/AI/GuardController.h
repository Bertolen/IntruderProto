// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GuardController.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API AGuardController : public AAIController
{
	GENERATED_BODY()

	// class constructor
	AGuardController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName PatrolLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName BotTypeKeyName;

public:
	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

	void SetTargetEnemy(APawn* NewTarget);

	//void SetBlackboardBotType(EBotBehaviorType NewType);
};
