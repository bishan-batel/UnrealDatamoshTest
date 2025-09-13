#pragma once

#include "CoreMinimal.h"
#include "RenderGraphUtils.h"
#include "SceneViewExtension.h"
#include "PostProcess/PostProcessMaterialInputs.h"

#include "DataDrivenShaderPlatformInfo.h"

class FDatamoshSceneViewExtension final : public FSceneViewExtensionBase
{
public:
	explicit FDatamoshSceneViewExtension(const FAutoRegister& AutoRegister);

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;

	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;

	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;

	virtual void SubscribeToPostProcessingPass(
		EPostProcessingPass Pass,
		FAfterPassCallbackDelegateArray&
		InOutPassCallbacks,
		bool bIsPassEnabled
	) override;


	FScreenPassTexture CustomPostProcessing(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View,
		const FPostProcessMaterialInputs& Inputs
	);
};

class DATAMOSHPLUGIN_API FDatamoshShader : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FDatamoshShader)
	SHADER_USE_PARAMETER_STRUCT(FDatamoshShader, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, OriginalSceneColor)
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, Output)
		SHADER_PARAMETER_STRUCT(FScreenPassTextureViewportParameters, SceneColorViewport)
	END_SHADER_PARAMETER_STRUCT()

	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);

	// Define environment variables used by compute shader
	static void ModifyCompilationEnvironment(
		const FGlobalShaderPermutationParameters& Parameters,
		FShaderCompilerEnvironment& OutEnvironment
	);
};
