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

AIntruderProtoHUD::AIntruderProtoHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}


void AIntruderProtoHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

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
