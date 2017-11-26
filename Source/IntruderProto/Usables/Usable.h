// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Usable.generated.h"

UCLASS()
class INTRUDERPROTO_API AUsable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsable(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	///////////////////////////////////////////////////////////////////////

public:
	// This function can be called to know if the object can be used or not by th given controller
	virtual bool CanBeUsed(AController* user);

	// This function will be called when the user uses the object
	virtual bool OnUsed(AController* user);

	// This function will be called when the user releases the object
	virtual void OnReleased(AController* user);

	// This function is called each frame by the HUD, it should be used to put messages to the screen, like the USE promt in UDK
	virtual void DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser);

	// Returns whether or not the item is busy
	bool IsUsed() const;

	// Returns the controller currently using the object
	FORCEINLINE AController* GetUser() const { return user; }
	
protected:
	// Usable label to display
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable", meta = (AllowPrivateAcces = "true"))
	FString UsableLabel;
	
private:
	// contains the current user
	UPROPERTY(VisibleAnywhere, Category = "Usable")
	AController* user;
};
