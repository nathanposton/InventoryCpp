// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Items/ItemBase.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

UItemBase* UInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindMatchingItemByID(UItemBase* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
			InventoryContents.
			FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UItemBase* UInventoryComponent::FindNextPartialStack(UItemBase* ItemIn) const
{
	if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result =
		InventoryContents.
		FindByPredicate(
			[ItemIn](const UItemBase* Item)
			{
				return Item->ID == ItemIn->ID && !Item->isFullItemStack();
			}))
	{
		return *Result;
	}
}

int32 UInventoryComponent::CalculateWeightAddAmount(
	UItemBase* ItemIn, int32 RequestedAddAmount)
{
	const int32 WeightMaxAddAmount = FMath::FloorToInt(
		(GetWeightCapacity() - InventoryTotalWeight) / ItemIn->
		GetItemSingleWeight());
	if (WeightMaxAddAmount >= RequestedAddAmount)
	{
		return RequestedAddAmount;
	}
	return WeightMaxAddAmount;
}

int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem,
	int32 InitialRequestedAddAmount)
{
	const int32 AddAmountToMakeFullStack = StackableItem->NumericData.
		MaxStackSize -
		StackableItem->Quantity;

	return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn,
                                              const int32 DesiredAmountToRemove)
{
	const int32 ActualAmountToRemove = FMath::Min(DesiredAmountToRemove,
	                                              ItemIn->Quantity);

	ItemIn->SetQuantity(ItemIn->Quantity - ActualAmountToRemove);

	InventoryTotalWeight -= ActualAmountToRemove * ItemIn->
		GetItemSingleWeight();

	OnInventoryUpdated.Broadcast();

	return ActualAmountToRemove;
}

void UInventoryComponent::SplitExistingStack(UItemBase* ItemIn,
                                             const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > InventorySlotsCapacity))
	{
		RemoveAmountOfItem(
			ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

FItemAddResult UInventoryComponent::HandleNonStackableItems(
	UItemBase* InputItem, int32 RequestedAddAmount)
{
	// check if input item has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemStackWeight()) || InputItem->
		GetItemStackWeight() < 0)
	{
		// return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(
				"Could not add {0} to inventory. Invalid weight."),
			FText::FromString(InputItem->TextData.Name)));
	}

	// check if inventory has enough space
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() >
		GetWeightCapacity())
	{
		// return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(
				"Could not add {0} to inventory. Not enough weight capacity."),
			FText::FromString(InputItem->TextData.Name)));
	}

	// check if inventory has enough slots
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		// return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(
				"Could not add {0} to inventory. Not enough inventory slots."),
			FText::FromString(InputItem->TextData.Name)));
	}

	AddNewItem(InputItem, RequestedAddAmount);
	// return handle added all result
	return FItemAddResult::AddedAll(RequestedAddAmount, FText::Format(
		                                FText::FromString(
			                                "Successfully added {0} {1} to the inventory."),
		                                RequestedAddAmount,
		                                FText::FromString(
			                                InputItem->TextData.Name)));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn,
                                                int32 RequestedAddAmount)
{
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// handle non-stackable
		if (InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem,
			                               InitialRequestedAddAmount);
		}

		// handle stackable
		const int32 StackableAmountAdded = HandleStackableItems(InputItem,
			InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			// return added all result
			// return FItemAddResult::Success;
		}
		if (StackableAmountAdded < InitialRequestedAddAmount &&
			StackableAmountAdded > 0)
		{
			// return added partial result
			// return FItemAddResult::Partial;
		}
		if (StackableAmountAdded <= 0)
		{
			// return failed result
			// return FItemAddResult::Failed;
		}
	}
}

void UInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
	UItemBase* NewItem = nullptr;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when splitting or dragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;
	NewItem->SetQuantity(AmountToAdd);

	InventoryContents.Add(NewItem);
	InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}
