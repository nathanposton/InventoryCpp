// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "InventoryCpp/InventoryCppCharacter.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYCPP_API UItemBase : public UObject
{
	GENERATED_BODY()
	
public:
	//=================================================================================================
	// PROPERTIES & VARIABLES
	//=================================================================================================
	// UPROPERTY()
	// UInventoryComponent* OwningInventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FName ID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EItemQuality Quality;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType Type;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FItemStatistics Statistics;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FItemTextData TextData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FItemNumericData NumericData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FItemAssetData AssetData;

	//=================================================================================================
	// FUNCTIONS
	//=================================================================================================
	UItemBase();

	UItemBase* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; }
	
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; }

	UFUNCTION(Category = "Item")
	FORCEINLINE bool isFullItemStack() const { return Quantity >= NumericData.MaxStackSize; }

	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	UFUNCTION(Category = "Item")
	virtual void Use(AInventoryCppCharacter* Character);

protected:
	bool operator==(const FName& OtherID) const
	{
		return this->ID == OtherID;
	};
};
