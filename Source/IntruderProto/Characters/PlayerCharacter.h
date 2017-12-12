// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usables/Usable.h"
#include "Characters/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class INTRUDERPROTO_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
	// Class constructor
	APlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

protected:
	
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	virtual void Jump() override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

public:
	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SetIsClimbingLadder(const bool &NewIsClimbingLadder);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/* MakeNoise hook to trigger AI noise emitting (Loudness between 0.0-1.0)  */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MakePawnNoise(float Loudness);

	// Inlines
	
	FORCEINLINE AUsable* GetFocusedUsable() const { return FocusedUsable; }

	FORCEINLINE bool GetIsClimbingLadder() const { return bIsClimbingLadder; }

	FORCEINLINE class AUsable* GetOnUseUsable() const { return OnUseUsable; }

	FORCEINLINE class UPhysicsHandleComponent* GetGrabHandle() const { return GrabHandle; }

	FORCEINLINE float GetThrowForce() const { return ThrowForce; }

	FORCEINLINE class UInventoryComponent* GetInventory() const { return Inventory; }

protected:
	// Usable item on focus
	AUsable* FocusedUsable;

	// Usable item currently being used by this actor
	AUsable* OnUseUsable;

	// This method uses the usable item on focus
	UFUNCTION(BlueprintCallable, Category = "Hability")
	virtual void Use();

	// This method uses the usable item on focus
	UFUNCTION(BlueprintCallable, Category = "Hability")
	virtual void Throw();

	// Scrolling the inventory
	UFUNCTION(BlueprintCallable, Category = "Hability")
	void ScrollInventory(float Value);

private:
	// is the character actually climbing a ladder?
	UPROPERTY()
	bool bIsClimbingLadder;

	// is the character actually climbing a ladder?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	float UsingReach;

	// Distance at which the player holds objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	float GrabDistance;

	// The force used to throw objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	float ThrowForce;

	//Physics handle used to grab objects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	class UPhysicsHandleComponent* GrabHandle;

	//Physics handle used to grab objects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

	/* Item currently equiped and ready to use */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APickup> EquipedItem;

	/** Item's spawn offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hability", meta = (AllowPrivateAccess = "true"))
	FVector ItemSpawnOffset;
};
