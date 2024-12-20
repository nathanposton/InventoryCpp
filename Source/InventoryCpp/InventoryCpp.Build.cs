// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventoryCpp : ModuleRules
{
	public InventoryCpp(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
