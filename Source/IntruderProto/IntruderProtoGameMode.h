// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IntruderProtoGameMode.generated.h"

UCLASS(minimalapi)
class AIntruderProtoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIntruderProtoGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/* Return the usable text */
	UFUNCTION(BlueprintPure, Category = "HUD")
	FString GetUsableText() const;

	/* Updates the usable text */
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetUsableText(const FString &NewText, float NewDisplaySpeed = -1);

protected:
	/** The Widget class to use for our HUD screen*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The instance of the HUD */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	FString UsableText;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	FString UsableTextFinal;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	float DisplaySpeed;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	float DisplayTimer;

	/* Updates the displayable text every frame */
	void UpdateDisplayText(float DeltaTime);
};



