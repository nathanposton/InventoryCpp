// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h" // Necessary if generated from UE5 Editor?

UENUM()
enum class EItemQuality : uint8
{
	Shoddy UMETA(DisplayName = "Shoddy"),
	Common UMETA(DisplayName = "Common"),
	Quality UMETA(DisplayName = "Quality"),
	Masterwork UMETA(DisplayName = "Masterwork"),
	Grandmaster UMETA(DisplayName = "Grandmaster"),
};

UENUM()
enum class EItemType : uint8
{
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest"),
	Mundane UMETA(DisplayName = "Mundane"),
};

// struct that allows blueprint
// solve error: /Users/nposton/Documents/Unreal Projects/InventoryCpp/Source/InventoryCpp/Public/ItemDataStructs.h(36) : Error: Cannot expose property to blueprints in a struct that is not a BlueprintType. Struct: FItemStatistics Property: Damage
USTRUCT(BlueprintType)
struct FItemStatistics
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Durability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CritChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CritMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Dexterity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Intelligence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Wisdom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Charisma;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Luck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Perception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Resistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Evasion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MagicFind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GoldFind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExperienceGain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ManaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaminaRegen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoisonResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IceResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightningResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EarthResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WindResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaterResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DarkResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PoisonDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IceDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LightningDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EarthDamage;
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText UsageText;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	FItemData()
	{
		ID = FName(*FGuid::NewGuid().ToString());
	}

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
};

// /**
//  * 
//  */
// class INVENTORYCPP_API ItemDataStructs
// {
// public:
// 	ItemDataStructs();
// 	~ItemDataStructs();
// };
