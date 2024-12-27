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
				return Item->ID == ItemIn->ID && !Item->IsFullItemStack();
			}))
	{
		return *Result;
	}
	return nullptr;
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
	UItemBase* InputItem)
{
	// check if input item has valid weight
	if (FMath::IsNearlyZero(InputItem->GetItemStackWeight()) || InputItem->
		GetItemStackWeight() < 0)
	{
		// return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(
				TEXT("Could not add {0} to inventory. Invalid weight.")),
			InputItem->TextData.Name));
	}

	// check if inventory has enough space
	if (InventoryTotalWeight + InputItem->GetItemSingleWeight() >
		GetWeightCapacity())
	{
		// return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(
				TEXT(
					"Could not add {0} to inventory. Not enough weight capacity.")),
			InputItem->TextData.Name));
	}

	// check if inventory has enough slots
	if (InventoryContents.Num() + 1 > InventorySlotsCapacity)
	{
		// return added none
		return FItemAddResult::AddedNone(FText::Format(
			FText::FromString(
				TEXT(
					"Could not add {0} to inventory. Not enough inventory slots.")),
			InputItem->TextData.Name));
	}

	AddNewItem(InputItem, 1);
	// return handle added all result
	return FItemAddResult::AddedAll(1, FText::Format(
		                                FText::FromString(
			                                TEXT(
				                                "Successfully added {0} {1} to the inventory.")),
		                                1,
		                                InputItem->TextData.Name));
}

int32 UInventoryComponent::HandleStackableItems(UItemBase* ItemIn,
                                                int32 RequestedAddAmount)
{
	// check if requested amount is valid
	if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(
		ItemIn->GetItemStackWeight()))
	{
		return 0;
	}

	int32 AmountToDistribute = RequestedAddAmount;
	UItemBase* ExistingItemStack = FindNextPartialStack(ItemIn);

	// add to any partial stacks first
	while (ExistingItemStack)
	{
		const int32 AmountToMakeFullStack = CalculateNumberForFullStack(
			ExistingItemStack, AmountToDistribute);
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(
			ExistingItemStack, AmountToMakeFullStack);

		// add if there is weight capacity
		if (WeightLimitAddAmount > 0)
		{
			ExistingItemStack->SetQuantity(ExistingItemStack->Quantity +
				WeightLimitAddAmount);
			InventoryTotalWeight += (WeightLimitAddAmount * ExistingItemStack->
				GetItemSingleWeight());

			AmountToDistribute -= WeightLimitAddAmount;

			ItemIn->SetQuantity(AmountToDistribute);

			// if weight limit reached/exceeded, halt the loop
			//TODO: Refine this logic since going over weight capacity should not be possible
			if (InventoryTotalWeight >= InventoryWeightCapacity)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}
		}
		// if no more weight capacity, halt the loop
		else if (WeightLimitAddAmount <= 0)
		{
			if (AmountToDistribute != RequestedAddAmount)
			{
				OnInventoryUpdated.Broadcast();
				return RequestedAddAmount - AmountToDistribute;
			}

			return 0;
		}

		// all of the input item was distributed across stacks
		if (AmountToDistribute <= 0)
		{
			OnInventoryUpdated.Broadcast();
			return RequestedAddAmount;
		}

		// current partial stack filled, still more to distribute. Get next partial stack
		ExistingItemStack = FindNextPartialStack(ItemIn);
	}

	// no more partial stacks found, check if a new stack can be added
	if (InventoryContents.Num() + 1 <= InventorySlotsCapacity)
	{
		const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ItemIn, AmountToDistribute);

		if (WeightLimitAddAmount > 0)
		{
			if (WeightLimitAddAmount < AmountToDistribute)
			{
				// create a partial stack up to amount that won't exceed weight
				AmountToDistribute -= WeightLimitAddAmount;
				ItemIn->SetQuantity(AmountToDistribute);

				// create a copy, because pickup will remain.
				AddNewItem(ItemIn->CreateItemCopy(), WeightLimitAddAmount);
				return RequestedAddAmount - AmountToDistribute;
			}

			// full	stack can be added
			AddNewItem(ItemIn, AmountToDistribute);
			return RequestedAddAmount;
		}
	}

	OnInventoryUpdated.Broadcast();
	return RequestedAddAmount - AmountToDistribute;
}

FItemAddResult UInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
	if (GetOwner())
	{
		const int32 InitialRequestedAddAmount = InputItem->Quantity;

		// handle non-stackable
		if (!InputItem->NumericData.bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}

		// handle stackable
		const int32 StackableAmountAdded = HandleStackableItems(InputItem,
			InitialRequestedAddAmount);

		if (StackableAmountAdded == InitialRequestedAddAmount)
		{
			// return added all result
			return FItemAddResult::AddedAll(InitialRequestedAddAmount,
			                                FText::Format(
				                                FText::FromString(
					                                TEXT(
						                                "Successfully added {0} {1} to the inventory.")),
				                                InitialRequestedAddAmount,
				                                InputItem->TextData.Name));
		}
		if (StackableAmountAdded < InitialRequestedAddAmount &&
			StackableAmountAdded > 0)
		{
			// return added partial result
			return FItemAddResult::AddedPartial(InitialRequestedAddAmount,
			                                    FText::Format(
				                                    FText::FromString(
					                                    TEXT(
						                                    "Partial amount ({0}) of {1} to the inventory.")),
				                                    InitialRequestedAddAmount,
				                                    InputItem->TextData.
				                                    Name));
		}
		if (StackableAmountAdded <= 0)
		{
			// return failed result
			return FItemAddResult::AddedNone(FText::Format(
				FText::FromString(
					TEXT(
						"Couldn't add {0} to the inventory. No remaining inventory slots or invalid item")),
				InputItem->TextData.Name));
		}
	}

	return FItemAddResult::AddedNone(FText::Format(
		FText::FromString(
			TEXT(
				"Couldn't add {0} to the inventory. Couldn't get owner. Or other edge case.")),
		InputItem->TextData.Name));
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
