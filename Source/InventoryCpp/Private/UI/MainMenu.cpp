// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu.h"

#include "InventoryCpp/InventoryCppCharacter.h"
#include "Items/ItemBase.h"
#include "UI/Inventory/ItemDragDropOperation.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<AInventoryCppCharacter>(GetOwningPlayerPawn());
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDropOperation->SourceItem)
	{
		PlayerCharacter->DropItem(ItemDragDropOperation->SourceItem, ItemDragDropOperation->SourceItem->Quantity);
		return true;
	}
	return false;
}
