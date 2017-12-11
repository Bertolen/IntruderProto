// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

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

bool UInventoryComponent::AddItem(TSubclassOf<APickup> ItemClass, int Amount)
{
	int index;
	if (Items.Find(ItemClass, index)) // If a slot is already open
	{
		ItemsCarried[index] += Amount;
		return true;
	}
	else if (Items.Num() < Size) { // If there's still an open slot
		Items.Add(ItemClass);
		ItemsCarried.Add(Amount);
		return true;
	}
	return false;
}

/* Removes items from the inventory, returns whether or not there are still some items of this kind in the inventory */
bool UInventoryComponent::RemoveItem(TSubclassOf<APickup> ItemClass, int Amount)
{
	int index;
	if (Items.Find(ItemClass, index)) // If a slot is already open
	{
		ItemsCarried[index] -= Amount;

		if (ItemsCarried[index] <= 0) { // if there's no more of these items
			ItemsCarried.RemoveAt(index);
			Items.RemoveAt(index);
			return false;
		}
		return true;
	}
	return false;
}

int UInventoryComponent::GetIndex(TSubclassOf<APickup> ItemClass) const
{
	return Items.Find(ItemClass);
}

TSubclassOf<APickup> UInventoryComponent::GetItemClassAtIndex(int Index) const
{
	if (Index < 0)
		return nullptr;

	if (Index >= Items.Num())
		return nullptr;

	return Items[Index];
}