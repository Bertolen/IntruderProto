// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Usables/Pickups/Pickup.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTRUDERPROTO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	bool AddItem(APickup* NewItem, int Amount = 1);

	void RemoveItem(APickup* NewItem, int Amount = 1);

	int GetIndex(TSubclassOf<APickup> ItemClass) const;

	TSubclassOf<APickup> GetItemClassAtIndex(int Index) const;

	// Inlines
	FORCEINLINE int GetMaxSize() const { return Size; }

	FORCEINLINE int GetUsedSlots() const { return Items.Num(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/* Inventory size, the amount of slots we have*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int Size;

	UPROPERTY()
	TArray<TSubclassOf<APickup>> Items;

	UPROPERTY()
	TArray<int> ItemsCarried;
};
