// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Usable.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API ADoor : public AUsable
{
	GENERATED_BODY()
	
	ADoor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	// This function will be called when the user uses the object
	virtual bool OnUsed(AController* NewUser);

	// This function is called each frame by the HUD, it should be used to put messages to the screen, like the USE promt in UDK
	virtual void DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser) override;
	
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called when the actor is placed on the world
	virtual void BeginPlay() override;

	// Returns the target rotator when the door is open
	FRotator GetOpenRotator() const;

	FORCEINLINE bool GetIsOpen() const { return IsOpen; }

private:
	/** Static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* DoorMesh;	

	/** Indicates if the door opens clockwise*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Door", meta = (AllowPrivateAccess = "true"))
	bool OpensClockwise;

	/** Indicates if the door is currently open*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable|Door", meta = (AllowPrivateAccess = "true"))
	bool IsOpen;

	/** Indicates if the door is currently open*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable|Door", meta = (AllowPrivateAccess = "true"))
	FQuat OpenQuat;

	/** Door rotation speed in degrees par second*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Door", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;
};
