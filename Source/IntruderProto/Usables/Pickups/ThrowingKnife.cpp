// Fill out your copyright notice in the Description page of Project Settings.

#include "ThrowingKnife.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/StaticMeshComponent.h"

AThrowingKnife::AThrowingKnife(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UsableLabel = "throwing knife";

	ImpulseForce = 10000.0f;
}

void AThrowingKnife::OnReleased(AController* User)
{
	UE_LOG(LogClass, Log, TEXT("A throwing knife has been released."));

	//SetActorRotation(GetActorRotation() + FRotator(0.0f, 0.0f, 90.0f));
	SetActorRotation(User->GetControlRotation() + FRotator(0.0f, 0.0f, 90.0f));

	PickupMesh->AddImpulse(User->GetControlRotation().Vector() * ImpulseForce);
}