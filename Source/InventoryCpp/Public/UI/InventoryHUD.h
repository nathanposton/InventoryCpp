// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryHUD.generated.h"

struct FInteractableData;
class UMainMenu;
class UInteractionWidget;

/**
 * 
 */
UCLASS()
class INVENTORYCPP_API AInventoryHUD : public AHUD
{
	GENERATED_BODY()

public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;
	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	AInventoryHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:

	// =================================================================================================
	// PROPERTIES & VARIABLES
	// =================================================================================================
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	// =================================================================================================
	// FUNCTIONS
	// =================================================================================================
	virtual void BeginPlay() override;

	
};
