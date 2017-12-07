// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IntruderProtoHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "IntruderProtoGameMode.h"

AIntruderProtoHUD::AIntruderProtoHUD() : Super()
{
	
}


void AIntruderProtoHUD::DrawHUD()
{
	Super::DrawHUD();

	// Display the usable text
	APlayerCharacter* player = Cast<APlayerCharacter>(GetOwningPawn());
	if (player && player->GetFocusedUsable() != NULL) {
		player->GetFocusedUsable()->DisplayPrompt(Canvas, GetOwningPlayerController());
	}
	else {
		AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode) {
			gameMode->SetUsableText(FString(""));
		}
	}
}
