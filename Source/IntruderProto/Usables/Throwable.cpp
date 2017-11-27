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

bool AThrowable::OnUsed(AController* user)
{
	if (!Super::OnUsed(user))
		return false;

	AIntruderProtoCharacter *character = Cast<AIntruderProtoCharacter>(user->GetCharacter());
	if (!character)
		return false;

	//character->GetGrabHandle()->GrabComponent(UsableMesh, FName(""), GetActorLocation(), true);
	character->GetGrabHandle()->GrabComponentAtLocation(UsableMesh, FName(""), GetActorLocation());

	return true;
}

void AThrowable::OnReleased(AController* user)
{
	Super::OnReleased(user);

	AIntruderProtoCharacter *character = Cast<AIntruderProtoCharacter>(user->GetCharacter());
	if (!character)
		return;

	character->GetGrabHandle()->ReleaseComponent();
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
		FString key = playerController->PlayerInput->GetKeysForAction("Throw")[0].Key.ToString();
		FString text = key + " : throw " + UsableLabel;

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