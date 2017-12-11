// Fill out your copyright notice in the Description page of Project Settings.

#include "ThrowingKnife.h"

AThrowingKnife::AThrowingKnife(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UsableLabel = "throwing knife";
}

void AThrowingKnife::OnReleased(AController* OldUser)
{
	UE_LOG(LogClass, Log, TEXT("A throwing knife has been released."));
}