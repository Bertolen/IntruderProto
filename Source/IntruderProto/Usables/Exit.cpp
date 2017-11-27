// Fill out your copyright notice in the Description page of Project Settings.

#include "Exit.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "IntruderProtoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AExit::AExit(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UsableLabel = "exit";

	// Create the static mesh component
	ExitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExitMesh"));
	ExitMesh->SetSimulatePhysics(false);
	ExitMesh->SetupAttachment(GetRootComponent());
}

bool AExit::OnUsed(AController* NewUser)
{
	if (!Super::OnUsed(NewUser) || LevelDestination == "")
		return false;

	UGameplayStatics::OpenLevel(GetWorld(), LevelDestination);

	return true;
}

void AExit::DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser)
{
	// We only display something if the usable object can be used
	if (!IsUsed() || GetUser() != DisplayerUser) {
		// The non players actors should not display anything
		APlayerController* playerController = Cast<APlayerController>(DisplayerUser);
		if (!playerController) {
			return;
		}

		// prepare the dynamic string to display
		FString key = playerController->PlayerInput->GetKeysForAction("Use")[0].Key.ToString();
		FString text = key + " : leave through " + UsableLabel;

		AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!gameMode) {
			return;
		}
		gameMode->SetUsableText(text);
	}
}