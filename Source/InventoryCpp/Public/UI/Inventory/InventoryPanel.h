// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class AInventoryCppCharacter;
class UWrapBox;
class UInventoryItemSlot;
class UTextBlock;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORYCPP_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

	// =================================================================================================
	// PROPERTIES & VARIABLES
	// =================================================================================================
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeightInfo;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityInfo;

	UPROPERTY()
	AInventoryCppCharacter* PlayerCharacter;

	UPROPERTY()
	UInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryItemSlot> InventorySlotClass;

	

	// =================================================================================================
	// FUNCTIONS
	// =================================================================================================
	UFUNCTION()
	void RefreshInventory();

protected:
	// =================================================================================================
	// PROPERTIES & VARIABLES
	// =================================================================================================

	// =================================================================================================
	// FUNCTIONS
	// =================================================================================================
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
};
