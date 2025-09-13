// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "DatamoshSceneViewExtension/Private/DatamoshSubsystem.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeDatamoshSubsystem() {}

// Begin Cross Module References
DATAMOSHPLUGIN_API UClass* Z_Construct_UClass_UDatamoshSubsystem();
DATAMOSHPLUGIN_API UClass* Z_Construct_UClass_UDatamoshSubsystem_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UEngineSubsystem();
UPackage* Z_Construct_UPackage__Script_DatamoshPlugin();
// End Cross Module References

// Begin Class UDatamoshSubsystem
void UDatamoshSubsystem::StaticRegisterNativesUDatamoshSubsystem()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UDatamoshSubsystem);
UClass* Z_Construct_UClass_UDatamoshSubsystem_NoRegister()
{
	return UDatamoshSubsystem::StaticClass();
}
struct Z_Construct_UClass_UDatamoshSubsystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "DatamoshSubsystem.h" },
		{ "ModuleRelativePath", "Private/DatamoshSubsystem.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UDatamoshSubsystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UDatamoshSubsystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UEngineSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_DatamoshPlugin,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UDatamoshSubsystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UDatamoshSubsystem_Statics::ClassParams = {
	&UDatamoshSubsystem::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UDatamoshSubsystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UDatamoshSubsystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UDatamoshSubsystem()
{
	if (!Z_Registration_Info_UClass_UDatamoshSubsystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UDatamoshSubsystem.OuterSingleton, Z_Construct_UClass_UDatamoshSubsystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UDatamoshSubsystem.OuterSingleton;
}
template<> DATAMOSHPLUGIN_API UClass* StaticClass<UDatamoshSubsystem>()
{
	return UDatamoshSubsystem::StaticClass();
}
UDatamoshSubsystem::UDatamoshSubsystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UDatamoshSubsystem);
UDatamoshSubsystem::~UDatamoshSubsystem() {}
// End Class UDatamoshSubsystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_schro_Documents_Unreal_Projects_DatamoshTest_Plugins_DatamoshPlugin_Source_DatamoshSceneViewExtension_Private_DatamoshSubsystem_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UDatamoshSubsystem, UDatamoshSubsystem::StaticClass, TEXT("UDatamoshSubsystem"), &Z_Registration_Info_UClass_UDatamoshSubsystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UDatamoshSubsystem), 160224010U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_schro_Documents_Unreal_Projects_DatamoshTest_Plugins_DatamoshPlugin_Source_DatamoshSceneViewExtension_Private_DatamoshSubsystem_h_2332653403(TEXT("/Script/DatamoshPlugin"),
	Z_CompiledInDeferFile_FID_Users_schro_Documents_Unreal_Projects_DatamoshTest_Plugins_DatamoshPlugin_Source_DatamoshSceneViewExtension_Private_DatamoshSubsystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_schro_Documents_Unreal_Projects_DatamoshTest_Plugins_DatamoshPlugin_Source_DatamoshSceneViewExtension_Private_DatamoshSubsystem_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
