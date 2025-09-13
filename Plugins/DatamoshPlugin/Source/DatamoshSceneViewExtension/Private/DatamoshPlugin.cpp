// Copyright Epic Games, Inc. All Rights Reserved.

#include "DatamoshPlugin.h"

#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FDatamoshSceneViewExtensionModule"

void FDatamoshPluginExtensionModule::StartupModule()
{
	FString PluginShaderDir = FPaths::Combine(
		IPluginManager::Get().FindPlugin(TEXT("DatamoshPlugin"))->GetBaseDir(),
		TEXT("Shaders")
	);

	AddShaderSourceDirectoryMapping(
		TEXT("/Plugins/DatamoshPlugin"),
		PluginShaderDir
	);
}

void FDatamoshPluginExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDatamoshPluginExtensionModule, DatamoshSceneViewExtension)
