// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

// enum to store the customs movement modes
UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	EUnknown,
	EClimbingLadder
};

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	// Crouching
	virtual void Crouch(bool bClientSimulation) override;

	// Uncrouching
	virtual void UnCrouch(bool bClientSimulation) override;
	
protected:

	// Tick
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Vertical camera movement when crouching and uncrouching
	UPROPERTY(EditDefaultsOnly, Category = "Character Movement (General Settings)")
	float CrouchingSpeed;

	// DeltaTime loaded into a variable so our (un)crouching methods can access it
	UPROPERTY()
	float SavedDeltaTime;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	// The physics rules to be applied if we use a custom movement mode
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	// The physics to be applied when climbing a ladder
	virtual void PhysClimbingLadder(float deltaTime, int32 Iterations);
};
