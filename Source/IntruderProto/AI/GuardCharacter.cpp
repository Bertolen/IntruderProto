// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardCharacter.h"
#include "GuardController.h"
// Sensing Components
#include "Perception/PawnSensingComponent.h"

// Sets default values
AGuardCharacter::AGuardCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PerceptionComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PerceptionComponent"));
	if (!PerceptionComponent) {
		return;
	}
	PerceptionComponent->SetPeripheralVisionAngle(60.0f);
	PerceptionComponent->SightRadius = 2000;
	PerceptionComponent->HearingThreshold = 600;
	PerceptionComponent->LOSHearingThreshold = 1200;

	bTargetSeen = false;
	SenseTimeOut = 0.6f;
	LastSeenTime = 0.0f;
}

// Called when the game starts or when spawned
void AGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (PerceptionComponent)
	{
		PerceptionComponent->OnSeePawn.AddDynamic(this, &AGuardCharacter::OnSeePlayer);
		PerceptionComponent->OnHearNoise.AddDynamic(this, &AGuardCharacter::OnHearNoise);
	}
}

// Called every frame
void AGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Check if we saw the target duringthis frame. If not we update to blackboard so that we don't follow the target for ever 
	if (bTargetSeen && (GetWorld()->GetTimeSeconds() - LastSeenTime) > SenseTimeOut) { 
		AGuardController* GuardController = Cast<AGuardController>(GetController());
		if (GuardController) { // Reset 
			bTargetSeen = false;
			GuardController->SetTargetToFollow(nullptr);
		}
	}
}

// Called to bind functionality to input
void AGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGuardCharacter::OnSeePlayer(APawn* Pawn)
{
	AGuardController* GuardController = Cast<AGuardController>(GetController());
	if (GuardController) {
		bTargetSeen = true;
		GuardController->SetTargetLocation(Pawn->GetActorLocation());
		GuardController->SetTargetToFollow(Pawn);
		LastSeenTime = GetWorld()->GetTimeSeconds();
	}
}

void AGuardCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	AGuardController* GuardController = Cast<AGuardController>(GetController());
	if (GuardController) {
		GuardController->SetTargetLocation(Location);
	}
}
