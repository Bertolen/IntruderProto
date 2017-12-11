// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Usable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API APickup : public AUsable
{
	GENERATED_BODY()
	
public:
	APickup(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// This function is called each frame by the HUD, it should be used to put messages to the screen, like the USE prompt in UDK
	virtual void DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser) override;

	// This function will be called when the user uses the object, it returns true if the user equips the object
	virtual bool OnUsed(AController* NewUser) override;

private:
	/** Static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;
	
};
