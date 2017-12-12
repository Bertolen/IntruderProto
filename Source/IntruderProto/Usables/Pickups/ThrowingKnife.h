// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Pickups/Pickup.h"
#include "ThrowingKnife.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API AThrowingKnife : public APickup
{
	GENERATED_BODY()
	
public:
	AThrowingKnife(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// This function will be called when the user releases the object
	virtual void OnEquipedUse(AController* User) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ImpulseForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FRotator SpawnRotationOffset;
};
