// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Pickup.generated.h"

class UItemBase;

UCLASS()
class INVENTORYCPP_API APickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:
	// ========================================================================
	// PROPERTIES & VARIABLES
	// ========================================================================

	// ========================================================================
	// FUNCTIONS
	// ========================================================================
	APickup();

	void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

	void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

	FORCEINLINE UItemBase* GetItemData() const { return ItemReference; }

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:
	// ========================================================================
	// PROPERTIES & VARIABLES
	// ========================================================================
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	UDataTable* ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	UItemBase* ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;
	
	// ========================================================================
	// FUNCTIONS
	// ========================================================================
	virtual void BeginPlay() override;
	
	virtual void Interact(AInventoryCppCharacter* PlayerCharacter) override;
	void UpdateInteractableData();
	
	void TakePickup(const AInventoryCppCharacter* Taker);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};