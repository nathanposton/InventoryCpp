// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu.h"

#include "InventoryCpp/InventoryCppCharacter.h"

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
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	// cast operation to item drag drop
	// ensure player is valid
	// call drop item on player
}
