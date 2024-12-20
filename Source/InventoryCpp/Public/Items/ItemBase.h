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
	
	UPROPERTY(VisibleAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 100))
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EItemQuality Quality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EItemType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemStatistics Statistics;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
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
		return ID == OtherID;
	};
};
