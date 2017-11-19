// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Usable.h"
#include "Ladder.generated.h"

class UInstancedStaticMeshComponent;

/**
 * 
 */
UCLASS()
class INTRUDERPROTO_API ALadder : public AUsable
{
	GENERATED_BODY()

public:
	ALadder();
	
public:

	virtual bool CanBeUsed(AController* NewUser) override;

	// This function will be called when the user uses the object
	virtual bool OnUsed(AController* NewUser) override;

	virtual void OnConstruction(const FTransform &Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder", meta = (AllowPrivateAccess = "true"))
	int AmountLadderMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder", meta = (AllowPrivateAccess = "true"))
	float MeshHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder", meta = (AllowPrivateAccess = "true"))
	float ReachX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder", meta = (AllowPrivateAccess = "true"))
	float ReachY;

protected:
	// The trigger space inside which the player can climb the ladder
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	// The meshes for our procedural generation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable|Meshes")
	UInstancedStaticMeshComponent* LadderStraight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable|Meshes")
	UInstancedStaticMeshComponent* LadderAttach;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable|Meshes")
	UInstancedStaticMeshComponent* LadderTop;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable|Meshes")
	UInstancedStaticMeshComponent* LadderBottom;

private:
	// The trigger space inside which the player can climb the ladder
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;
};
