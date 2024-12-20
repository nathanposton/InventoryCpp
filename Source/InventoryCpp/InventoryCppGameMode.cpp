// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryCppGameMode.h"
#include "InventoryCppCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInventoryCppGameMode::AInventoryCppGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/InventorySystem/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
