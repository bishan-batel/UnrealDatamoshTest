// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class DatamoshPlugin : ModuleRules
{
	public DatamoshPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(new[]
		{
			Path.Combine(EngineDirectory, "Source/Runtime/Renderer/Public"),
			Path.Combine(EngineDirectory, "Source/Runtime/Renderer/Internal"),
		});

		PublicDependencyModuleNames.AddRange(new[] { "Core", });

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"RenderCore",
			"Renderer",
			"RHI",
			"Projects"
		});


		// DynamicallyLoadedModuleNames.AddRange(new[] { });
	}
}