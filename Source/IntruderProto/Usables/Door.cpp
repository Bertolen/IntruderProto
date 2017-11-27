// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "IntruderProtoGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ADoor::ADoor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UsableLabel = "door";

	// Allows this actor to tick now
	PrimaryActorTick.bCanEverTick = true;

	// Create the static mesh component
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(GetRootComponent());

	// default values for our variables
	IsOpen = false;
	OpensClockwise = false;
	RotationSpeed = 90.0f;
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//The door can't tick, yet
	SetActorTickEnabled(false);
}

void ADoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Calculates the total rotation left to do
	FRotator TotalRot;

	if (IsOpen) {
		TotalRot = GetOpenRotator() - DoorMesh->RelativeRotation;
	}
	else {
		TotalRot = FRotator::ZeroRotator - DoorMesh->RelativeRotation;
	}

	// Calculates the rotation to do during this frame
	float DeltaSpeed;

	if (TotalRot.Yaw != 0.0f) {
		DeltaSpeed = FMath::DegreesToRadians(RotationSpeed) * DeltaSeconds * TotalRot.Yaw / FMath::Abs(TotalRot.Yaw);
	}
	else {
		DeltaSpeed = 0.0f;
		SetActorTickEnabled(false);
	}

	FRotator DeltaRotation = FRotator(FQuat(FVector::UpVector, DeltaSpeed));
	
	// Avoids overshoots and stops the ticking
	if (FMath::Abs(DeltaRotation.Yaw) > FMath::Abs(TotalRot.Yaw)) { 
		DeltaRotation = TotalRot;
		SetActorTickEnabled(false);
	}

	// Applies the rotation
	DoorMesh->AddRelativeRotation(DeltaRotation);
}

bool ADoor::OnUsed(AController* NewUser)
{
	if (!Super::OnUsed(NewUser))
		return false;
	
	//toggle the state
	IsOpen = !IsOpen;

	// The door will tick, for now
	SetActorTickEnabled(true);

	// the door is always available
	OnReleased(NewUser);

	//send a false failure so the player won't keep the door as under use and won't try to release it later
	return false;
}

void ADoor::DisplayPrompt(UCanvas* Canvas, AController* DisplayerUser)
{
	// The non players actors should not display anything
	APlayerController* playerController = Cast<APlayerController>(DisplayerUser);
	if (!playerController) {
		return;
	}

	// prepare the dynamic string to display
	FString key = playerController->PlayerInput->GetKeysForAction("Use")[0].Key.ToString();
	FString text = key + " : open " + UsableLabel;

	AIntruderProtoGameMode* gameMode = Cast<AIntruderProtoGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!gameMode) {
		return;
	}
	gameMode->SetUsableText(text);
}

FRotator ADoor::GetOpenRotator() const
{
	if (OpensClockwise) {
		return FRotator(FQuat(FVector::UpVector, FMath::DegreesToRadians(-90.0f)));
	}
	else {
		return FRotator(FQuat(FVector::UpVector, FMath::DegreesToRadians(90.0f)));
	}
}