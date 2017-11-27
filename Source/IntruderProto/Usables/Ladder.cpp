// Fill out your copyright notice in the Description page of Project Settings.

#include "Ladder.h"
#include "GameFramework/PlayerController.h"
#include "IntruderProtoCharacter.h"
#include "MyCharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ALadder::ALadder()
{
	// The label to be used to display
	UsableLabel = "ladder";

	// Root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	// Setup the climb volume
	ClimbVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("ClimbVolume"));
	ClimbVolume->SetupAttachment(GetRootComponent());
	ClimbVolume->bDynamicObstacle = true;
	ClimbVolume->bGenerateOverlapEvents = true;
	ClimbVolume->SetCollisionResponseToAllChannels(ECR_Overlap);

	// Setup the meshes 
	LadderStraight = CreateAbstractDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("LadderStraight"));
	LadderStraight->SetupAttachment(GetRootComponent());
	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/Meshes/Ladder_Straight.Ladder_Straight'"));
	if (mesh.Succeeded()) {
		LadderStraight->SetStaticMesh(mesh.Object);
	}

	LadderAttach = CreateAbstractDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("LadderAttach"));
	LadderAttach->SetupAttachment(GetRootComponent());
	mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/Ladder_Attach.Ladder_Attach'"));
	if (mesh.Succeeded()) {
		LadderAttach->SetStaticMesh(mesh.Object);
	}

	LadderTop = CreateAbstractDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("LadderTop"));
	LadderTop->SetupAttachment(GetRootComponent());
	mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/Ladder_Top.Ladder_Top'"));
	if (mesh.Succeeded()) {
		LadderTop->SetStaticMesh(mesh.Object);
	}

	LadderBottom = CreateAbstractDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("LadderBottom"));
	LadderBottom->SetupAttachment(GetRootComponent());
	mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Meshes/Ladder_Bottom.Ladder_Bottom'"));
	if (mesh.Succeeded()) {
		LadderBottom->SetStaticMesh(mesh.Object);
	}

	//Setup the arrow
	InwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("InwardArrow"));
	InwardArrow->SetRelativeRotation(FQuat(FVector(0.0f, 0.0f, 1.0f), FMath::DegreesToRadians(-90.0f)));

	//init some values
	AmountLadderMeshes = 10;
	MeshHeight = 30.0f;
	ReachX = 40.0f;
	ReachY = 20.0f;
}

void ALadder::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	
	// Do some clean up first
	LadderStraight->ClearInstances();
	LadderAttach->ClearInstances();
	LadderBottom->ClearInstances();
	LadderTop->ClearInstances();

	// Now we start the real construction
	
	for (int i = 0; i < AmountLadderMeshes; i++)
	{
		if (i == 0) {
			LadderBottom->AddInstance(FTransform(FVector(0, 0, i*MeshHeight)));
		}
		else if (i == AmountLadderMeshes - 1) {
			LadderTop->AddInstance(FTransform(FVector(0, 0, i*MeshHeight)));
		}
		else if (i % 3 == 0) {
			LadderAttach->AddInstance(FTransform(FVector(0, 0, i*MeshHeight)));
		}
		else {
			LadderStraight->AddInstance(FTransform(FVector(0, 0, i*MeshHeight)));
		}
	}
	
	float boxCenter = AmountLadderMeshes * MeshHeight / 2;
	float boxHeight = boxCenter + 20.0f;
	FVector extents = ClimbVolume->Bounds.BoxExtent;
	extents.Z = boxHeight;
	extents.Y = ReachY;
	extents.X = ReachX;
	ClimbVolume->SetBoxExtent(extents);
	ClimbVolume->SetRelativeLocation(FVector(0.0f, 0.0f, boxCenter));
	InwardArrow->SetRelativeLocation(FVector(0.0f, 0.0f, boxCenter));
}

bool ALadder::CanBeUsed(AController* PromptUser)
{
	// if the user is already using the ladder he/she can't use it again.
	if (PromptUser == GetUser()) {
		return false;
	}

	// if the user is not overlapping the climb volume he/she can't use the ladder
	if (!ClimbVolume->IsOverlappingActor(PromptUser->GetCharacter())) {
		return false;
	}

	return true;
}

bool ALadder::OnUsed(AController* NewUser)
{
	if (!CanBeUsed(NewUser) || !Super::OnUsed(NewUser)) {
		OnReleased(NewUser);
		return false;
	}

	AIntruderProtoCharacter* character = Cast<AIntruderProtoCharacter>(NewUser->GetCharacter());
	if (!character) {
		OnReleased(NewUser);
		return false;
	}
	else {
		UMyCharacterMovementComponent* moveComp = Cast<UMyCharacterMovementComponent>(character->GetMovementComponent());
		if (!moveComp) {
			OnReleased(NewUser);
			return false;
		}
		else {
			moveComp->SetMovementMode(EMovementMode::MOVE_Custom, (uint8)ECustomMovementMode::EClimbingLadder);
			character->SetIsClimbingLadder(true);
		}
	}

	return true;
}