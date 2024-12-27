// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryPanel.h"

#include "Components/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "InventoryCpp/InventoryCppCharacter.h"
#include "UI/Inventory/InventoryItemSlot.h"
#include "UI/Inventory/ItemDragDropOperation.h"

void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PlayerCharacter = Cast<AInventoryCppCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		InventoryReference = PlayerCharacter->GetInventory();
		if (InventoryReference)
		{
			InventoryReference->OnInventoryUpdated.AddUObject(
				this, &UInventoryPanel::RefreshInventory);
			SetInfoText();
		}
	}
}

void UInventoryPanel::SetInfoText() const
{
	// WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	//                                   InventoryReference->
	//                                   GetInventoryTotalWeight(),
	//                                   InventoryReference->GetWeightCapacity()));
	WeightInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	                                  FText::AsNumber(
		                                  InventoryReference->
		                                  GetInventoryTotalWeight(),
		                                  &FNumberFormattingOptions().
		                                   SetMinimumFractionalDigits(1).
		                                   SetMaximumFractionalDigits(2)),
	                                  FText::AsNumber(
		                                  InventoryReference->
		                                  GetWeightCapacity(),
		                                  &FNumberFormattingOptions().
		                                   SetMinimumFractionalDigits(1).
		                                   SetMaximumFractionalDigits(2))));
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	                                    InventoryReference->GetInventoryItems().
	                                    Num(),
	                                    InventoryReference->
	                                    GetSlotsCapacity()));
}

void UInventoryPanel::RefreshInventory()
{
	if (InventoryReference && InventorySlotClass)
	{
		InventoryPanel->ClearChildren();

		for (const auto InventoryItem : InventoryReference->GetInventoryItems())
		{
			UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(
				this,
				InventorySlotClass);
			ItemSlot->SetItemReference(InventoryItem);
			InventoryPanel->AddChildToWrapBox(ItemSlot);
		}

		SetInfoText();
	}
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry,
                                   const FDragDropEvent& InDragDropEvent,
                                   UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (ItemDragDrop->SourceItem && InventoryReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detected an item drop on InventoryPanel."));

		// returning true will stop the drop operation at this widget
		return true;
	}
	// returning false will allow the drop operation to continue to the next widget
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
