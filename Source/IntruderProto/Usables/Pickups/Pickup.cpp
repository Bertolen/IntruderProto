// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"

#include "Characters/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "IntruderProtoGameMode.h"
#include "Kismet/GameplayStatics.h"

APickup::APickup(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// The label to be used to display
	UsableLabel = "pickup";
	
	// Create the static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetupAttachment(RootComponent);
}

void APickup::DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser)
{
	// The non players actors should not display anything
	APlayerController* playerController = Cast<APlayerController>(DisplayerUser);
	if (playerController) {
		// prepare the dynamic string to display
		FString key = playerController->PlayerInput->GetKeysForAction("Use")[0].Key.ToString();
		FString text = key + " : take " + UsableLabel;

		AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode) {
			gameMode->SetUsableText(text);
		}
	}
}

bool APickup::OnUsed(AController* NewUser)
{
	Super::OnUsed(NewUser);

	APlayerCharacter* character = Cast<APlayerCharacter>(NewUser->GetCharacter());
	if (!character) {
		OnReleased(NewUser);
		return false;
	}

	if (character->GetInventory()->AddItem(this->GetClass())) {
		Destroy();
	}

	return false;
}

void APickup::OnEquipedUse(AController * User)
{
	UE_LOG(LogClass, Log, TEXT("Item %s is equiped and used."), *GetFName().ToString());
}