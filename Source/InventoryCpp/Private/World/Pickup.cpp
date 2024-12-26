// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"

#include "Components/InventoryComponent.h"
#include "Items/ItemBase.h"

// Sets default values
APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("PickupMesh"));
	SetRootComponent(PickupMesh);
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

	InitializePickup(UItemBase::StaticClass(), ItemQuantity);
}

void APickup::InitializePickup(const TSubclassOf<UItemBase> BaseClass,
                               const int32 InQuantity)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(
			DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UItemBase>(this, BaseClass);

		ItemReference->ID = ItemData->ID;
		ItemReference->Type = ItemData->Type;
		ItemReference->Quality = ItemData->Quality;
		ItemReference->NumericData = ItemData->NumericData;
		ItemReference->TextData = ItemData->TextData;
		ItemReference->AssetData = ItemData->AssetData;
		ItemReference->Statistics = ItemData->Statistics;

		InQuantity <= 0
			? ItemReference->SetQuantity(1)
			: ItemReference->SetQuantity(InQuantity);

		PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);

		UpdateInteractableData();
	}
}

void APickup::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
	ItemReference = ItemToDrop;
	InQuantity <= 0
		? ItemReference->SetQuantity(1)
		: ItemReference->SetQuantity(InQuantity);
	// redundant?
	ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
	ItemReference->Statistics = ItemToDrop->Statistics;
	PickupMesh->SetStaticMesh(ItemToDrop->AssetData.Mesh);

	UpdateInteractableData();
	// log the numeric data weight at this point
	UE_LOG(LogTemp, Warning,
	       TEXT("InitializeDrop() ItemReference->NumericData.Weight: %f"),
	       ItemReference->NumericData.Weight);
}

void APickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
	InstanceInteractableData.Name = ItemReference->TextData.Name;
	InstanceInteractableData.Quantity = ItemReference->Quantity;
	InteractableData = InstanceInteractableData;
}

void APickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void APickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

void APickup::Interact(AInventoryCppCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		TakePickup(PlayerCharacter);
	}
}

void APickup::TakePickup(const AInventoryCppCharacter* Taker)
{
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (UInventoryComponent* PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(
					ItemReference);

				UE_LOG(LogTemp, Warning, TEXT("TakePickup() AddResult: %s"),
				       *AddResult.ResultMessage.ToString());
				// log out the numeric data
				UE_LOG(LogTemp, Warning,
				       TEXT("TakePickup() ItemReference->NumericData.Weight: %f"
				       ), ItemReference->NumericData.Weight);
				// log out the statistic data
				UE_LOG(LogTemp, Warning,
				       TEXT("TakePickup() ItemReference->Statistics.Damage: %f"
				       ), ItemReference->Statistics.Damage);

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_NoItemsAdded:
					break;
				default:
					break;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning,
				       TEXT("TakePickup() PlayerInventory component is null."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("TakePickup() ItemReference is null."));
		}
	}
}

void APickup::PostEditChangeProperty(
	FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property
		                                  ? PropertyChangedEvent.Property->
		                                  GetFName()
		                                  : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickup, DesiredItemID))
	{
		if (ItemDataTable)
		{
			if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(
				DesiredItemID, DesiredItemID.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->AssetData.Mesh);
			}
		}
	}
}
