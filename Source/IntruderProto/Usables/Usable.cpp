// Fill out your copyright notice in the Description page of Project Settings.

#include "Usable.h"
#include "Engine/Canvas.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "IntruderProtoGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUsable::AUsable(const FObjectInitializer& ObjectInitializer) : Super (ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// The label to be used to display
	UsableLabel = "usable";

	// Init the user
	user = NULL;
}

bool AUsable::IsUsed() const
{
	if (user == NULL) {
		return false;
	}
	return true;
}

bool AUsable::CanBeUsed(AController* PromptUser)
{
	return !IsUsed();
}

bool AUsable::OnUsed(AController* NewUser)
{
	user = NewUser;
	return true;
}

void AUsable::OnReleased(AController* OldUser)
{
	if (user != OldUser) {
		UE_LOG(LogClass, Log, TEXT("ERROR : Releasing a usable from a controller that's not using it!!!"));
	}
	user = NULL;
}

void AUsable::DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser)
{
	// We only display something if the usable object can be used
	if (!CanBeUsed(DisplayerUser)) {
		AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode) {
			gameMode->SetUsableText(FString(""));
		}
	} 
	else {
		// The non players actors should not display anything
		APlayerController* playerController = Cast<APlayerController>(DisplayerUser);
		if (playerController) {
			// prepare the dynamic string to display
			FString key = playerController->PlayerInput->GetKeysForAction("Use")[0].Key.ToString();
			FString text = key + " : use " + UsableLabel;

			AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (gameMode) {
				gameMode->SetUsableText(text);
			}
		}
	}
}