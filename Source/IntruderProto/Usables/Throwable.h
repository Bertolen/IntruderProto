// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Usable.h"
#include "Throwable.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API AThrowable : public AUsable
{
	GENERATED_BODY()

public:
	AThrowable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// This function will be called when the user uses the object
	virtual bool OnUsed(AController* NewUser);

	// This function will be called when the user releases the object
	virtual void OnReleased(AController* Oldser) override;
	
	// This function will be called when the user throws the object
	virtual void OnThrow(AController* Oldser);

	// This function is called each frame by the HUD, it should be used to put messages to the screen, like the USE promt in UDK
	virtual void DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser) override;

private:
	/** Static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* UsableMesh;
};
