// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "IntruderProtoHUD.generated.h"

UCLASS()
class AIntruderProtoHUD : public AHUD
{
	GENERATED_BODY()

public:
	AIntruderProtoHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

};

