// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/InventoryTooltip.h"

#include "Items/ItemBase.h"
#include "UI/Inventory/InventoryItemSlot.h"

void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->
		GetItemReference();

	// initialize the tooltip with the item data
	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.Damage));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->Statistics.Armor));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	StackWeight->SetText(
		FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));
	SellValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.Value));

	// per type, hide irrelevant fields
	switch (ItemBeingHovered->Type)
	{
	case EItemType::Armor:
		break;
	case EItemType::Weapon:
		ItemType->SetText(FText::FromString("Weapon"));
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Shield:
		break;
	case EItemType::Spell:
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		SellValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		break;
	case EItemType::Mundane:
		break;
	default: ;
	}

	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(
			ItemBeingHovered->NumericData.MaxStackSize));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
