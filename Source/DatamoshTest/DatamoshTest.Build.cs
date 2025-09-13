// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DatamoshTest : ModuleRules
{
	public DatamoshTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
