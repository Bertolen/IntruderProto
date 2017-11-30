// Fill out your copyright notice in the Description page of Project Settings.

#include "Throwable.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "IntruderProto/IntruderProtoCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "IntruderProtoGameMode.h"
#include "Kismet/GameplayStatics.h"

AThrowable::AThrowable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UsableLabel = "default throwable";

	// Create the static mesh component
	UsableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UsableMesh"));
	UsableMesh->SetSimulatePhysics(true);
	UsableMesh->SetupAttachment(GetRootComponent());
}

bool AThrowable::OnUsed(AController* NewUser)
{
	if (!Super::OnUsed(NewUser))
		return false;

	AIntruderProtoCharacter *character = Cast<AIntruderProtoCharacter>(NewUser->GetCharacter());
	if (!character)
		return false;

	character->GetGrabHandle()->GrabComponentAtLocation(UsableMesh, FName(""), GetActorLocation());

	return true;
}

void AThrowable::OnReleased(AController* OldUser)
{
	Super::OnReleased(OldUser);

	AIntruderProtoCharacter *character = Cast<AIntruderProtoCharacter>(OldUser->GetCharacter());
	if (!character)
		return;

	character->GetGrabHandle()->ReleaseComponent();
}

void AThrowable::OnThrow(AController* OldUser)
{
	OnReleased(OldUser);

	AIntruderProtoCharacter *character = Cast<AIntruderProtoCharacter>(OldUser->GetCharacter());
	if (!character)
		return;

	UsableMesh->AddImpulse(character->GetFirstPersonCameraComponent()->GetForwardVector() * character->GetThrowForce());
}

void AThrowable::DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser)
{
	// We only display something if the usable object can be used
	if (IsUsed() && GetUser() == DisplayerUser) {
		// The non players actors should not display anything
		APlayerController* playerController = Cast<APlayerController>(DisplayerUser);
		if (!playerController) {
			return;
		}

		// prepare the dynamic string to display
		FString ThrowKey = playerController->PlayerInput->GetKeysForAction("Throw")[0].Key.ToString();
		FString ReleaseKey = playerController->PlayerInput->GetKeysForAction("Use")[0].Key.ToString();
		FString text = ThrowKey + " : throw " + UsableLabel + '\n' + ReleaseKey + " : release " + UsableLabel;

		AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!gameMode) {
			return;
		}
		gameMode->SetUsableText(text);
	}
	else {
		// The non players actors should not display anything
		APlayerController* playerController = Cast<APlayerController>(DisplayerUser);
		if (!playerController) {
			return;
		}

		// prepare the dynamic string to display
		FString key = playerController->PlayerInput->GetKeysForAction("Use")[0].Key.ToString();
		FString text = key + " : grab " + UsableLabel;

		AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (!gameMode) {
			return;
		}
		gameMode->SetUsableText(text);
	}
}