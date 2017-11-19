// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IntruderProtoGameMode.h"
#include "IntruderProtoHUD.h"
#include "IntruderProtoCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AIntruderProtoGameMode::AIntruderProtoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AIntruderProtoHUD::StaticClass();
}

void AIntruderProtoGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Set the HUD
	if (HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr) {
			CurrentWidget->AddToViewport();
		}
	}

	// Allows ticking
	PrimaryActorTick.bCanEverTick = true;
}

void AIntruderProtoGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateDisplayText(DeltaTime);
}

void AIntruderProtoGameMode::UpdateDisplayText(float DeltaTime)
{
	// Usable text
	if (UsableText != UsableTextFinal) {
		DisplayTimer += DeltaTime;
		int charactersAmount = (int) DisplaySpeed * DisplayTimer;
		UsableText = UsableTextFinal.Left(charactersAmount);
	}
}

FString AIntruderProtoGameMode::GetUsableText() const
{
	return UsableText;
}

void AIntruderProtoGameMode::SetUsableText(const FString &NewText, float NewDisplaySpeed)
{
	UsableTextFinal = NewText;
	DisplaySpeed = NewDisplaySpeed;

	if (DisplaySpeed < 0) {
		UsableText = UsableTextFinal;
	} 
	else {
		UsableText = 0;
		DisplayTimer = 0;
	}
}