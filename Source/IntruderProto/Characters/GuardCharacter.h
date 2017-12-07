// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "GuardCharacter.generated.h"

UCLASS()
class INTRUDERPROTO_API AGuardCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuardCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE TArray<FVector> GetWaypoints() const { return Waypoints; }

protected:
	// Called when the character see someone
	UFUNCTION()
	virtual void OnSeePlayer(APawn* Pawn);

	// Called when the character hears someone
	UFUNCTION()
	virtual void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* PerceptionComponent;

	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SenseTimeOut;

	/* Resets after sense time-out to avoid unnecessary clearing of target each tick */
	UPROPERTY()
	bool bTargetSeen;
	
	/* Last time the player was spotted */
	UPROPERTY()
	float LastSeenTime;

	/** List of waypoints the character will go through during his patrol **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> Waypoints;
};
