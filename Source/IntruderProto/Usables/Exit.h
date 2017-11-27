// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Usable.h"
#include "Exit.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API AExit : public AUsable
{
	GENERATED_BODY()
	
	AExit(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// This function will be called when the user uses the object
	virtual bool OnUsed(AController* user);

	// This function is called each frame by the HUD, it should be used to put messages to the screen, like the USE promt in UDK
	virtual void DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser) override;

private:
	/** Static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ExitMesh;

	/** Name of the level this exit leads to */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exit", meta = (AllowPrivateAccess = "true"))
	FName LevelDestination;
};
