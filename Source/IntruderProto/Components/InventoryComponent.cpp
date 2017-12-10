// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "Usables/Pickups/Pickup.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	Size = 5;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool UInventoryComponent::AddItem(APickup* NewItem, int Amount)
{
	int index;
	if (Items.Find(NewItem->StaticClass(), index)) // If a slot is already open
	{
		ItemsCarried[index] += Amount;
		return true;
	}
	else if (Items.Num() < Size) { // If there's still an open slot
		Items.AddUnique(NewItem->StaticClass());
		ItemsCarried.Add(Amount);
		return true;
	}
	return false;
}

void UInventoryComponent::RemoveItem(APickup* NewItem, int Amount)
{
	int index;
	if (Items.Find(NewItem->StaticClass(), index)) // If a slot is already open
	{
		ItemsCarried[index] -= Amount;

		if (ItemsCarried[index] <= 0) { // if there's no more of these items
			ItemsCarried.RemoveAt(index);
			Items.RemoveAt(index);
		}
	}
}