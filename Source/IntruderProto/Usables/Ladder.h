// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Usable.h"
#include "Components/ArrowComponent.h"
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

	FORCEINLINE FVector GetInward() const { return InwardArrow->GetComponentRotation().Vector(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder")
	int AmountLadderMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder")
	float MeshHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder")
	float ReachX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Usable|Ladder")
	float ReachY;

protected:
	// The trigger space inside which the player can climb the ladder
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable")
	class UBoxComponent* ClimbVolume;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Usable", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* InwardArrow;
};
