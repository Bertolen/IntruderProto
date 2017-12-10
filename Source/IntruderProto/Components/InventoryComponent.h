// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTRUDERPROTO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	bool AddItem(class APickup* NewItem, int Amount = 1);

	void RemoveItem(class APickup* NewItem, int Amount = 1);

	// Inlines
	FORCEINLINE int GetSize() const { return Size; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	/* Inventory size, the amount of slots we have*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int Size;

	UPROPERTY()
	TArray<TSubclassOf<class APickup>> Items;

	UPROPERTY()
	TArray<int> ItemsCarried;
};
