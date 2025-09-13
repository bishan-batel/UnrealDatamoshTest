#include "FDatamoshSceneViewExtension.h"

#include "PostProcess/PostProcessMaterialInputs.h"


IMPLEMENT_GLOBAL_SHADER(FDatamoshShader, "/Plugins/DatamoshPlugin/PostProcessCS.usf", "MainCS", SF_Compute);

namespace
{
	TAutoConsoleVariable<int32> CVarShaderOn{
		TEXT("r.DatamoshPlugin"),
		0,
		TEXT("Enable Custom SceneViewExtension \n")
		TEXT(" 0: OFF;")
		TEXT(" 1: ON."),
		ECVF_RenderThreadSafe
	};
}

FDatamoshSceneViewExtension::FDatamoshSceneViewExtension(const FAutoRegister& AutoRegister) :
	FSceneViewExtensionBase{AutoRegister}
{
	UE_LOG(LogTemp, Log, TEXT("Datamosh Plugin Registered"));
}

void FDatamoshSceneViewExtension::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FDatamoshSceneViewExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
}

void FDatamoshSceneViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FDatamoshSceneViewExtension::SubscribeToPostProcessingPass(
	const EPostProcessingPass Pass,
	FAfterPassCallbackDelegateArray& InOutPassCallbacks,
	const bool bIsPassEnabled
)
{
	if (Pass == EPostProcessingPass::MotionBlur)
	{
		InOutPassCallbacks.Add(
			FAfterPassCallbackDelegate::CreateRaw(this, &FDatamoshSceneViewExtension::CustomPostProcessing)
		);
	}
}

FScreenPassTexture FDatamoshSceneViewExtension::CustomPostProcessing(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessMaterialInputs& Inputs)
{
	const FSceneViewFamily& ViewFamily = *View.Family;

	const FScreenPassTexture& SceneColor = FScreenPassTexture::CopyFromSlice(
		GraphBuilder, Inputs.GetInput(EPostProcessMaterialInput::SceneColor)
	);

	if (!SceneColor.IsValid() or CVarShaderOn.GetValueOnRenderThread())
	{
		return SceneColor;
	}

	const FScreenPassTexture& SceneVelocity = FScreenPassTexture::CopyFromSlice(
		GraphBuilder, Inputs.GetInput(EPostProcessMaterialInput::Velocity)
	);

	const FScreenPassTextureViewport SceneColorViewport{SceneColor};
	const FScreenPassTextureViewport SceneVelocityViewport{SceneVelocity};

	RDG_EVENT_SCOPE(GraphBuilder, "Custom post process effect");

	// Accesspoint to our Shaders
	FGlobalShaderMap* GlobalShaderMap = GetGlobalShaderMap(ViewFamily.GetFeatureLevel());

	// Setup all the descriptors to create a target texture
	FRDGTextureDesc OutputDesc;
	{
		OutputDesc = SceneColor.Texture->Desc;

		OutputDesc.Reset();
		OutputDesc.Flags |= TexCreate_UAV;
		OutputDesc.Flags &= ~(TexCreate_RenderTargetable | TexCreate_FastVRAM);

		FLinearColor ClearColor(0., 0., 0., 0.);
		OutputDesc.ClearValue = FClearValueBinding(ClearColor);
	}

	// Create target texture
	FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(OutputDesc, TEXT("Custom Effect Output Texture"));

	// Set the shader parameters
	FDatamoshShader::FParameters* PassParameters = GraphBuilder.AllocParameters<FDatamoshShader::FParameters>();

	// Input is the SceneColor from PostProcess Material Inputs
	PassParameters->OriginalSceneColor = SceneColor.Texture;

	PassParameters->Velocity = SceneVelocity.Texture;

	// Use ScreenPassTextureViewportParameters so we don't need to calculate these ourselves
	PassParameters->SceneColorViewport = GetScreenPassTextureViewportParameters(SceneColorViewport);

	PassParameters->SceneVelocityViewport = GetScreenPassTextureViewportParameters(SceneVelocityViewport);

	const FIntPoint PassViewSize = SceneColor.ViewRect.Size();

	// Create UAV from Target Texture
	PassParameters->Output = GraphBuilder.CreateUAV(FRDGTextureUAVDesc(OutputTexture));

	// Set Compute Shader and execute
	const FIntVector GroupCount = FComputeShaderUtils::GetGroupCount(PassViewSize,
	                                                                 FComputeShaderUtils::kGolden2DGroupSize);

	TShaderMapRef<FDatamoshShader> ComputeShader{GlobalShaderMap};

	FComputeShaderUtils::AddPass(
		GraphBuilder,
		RDG_EVENT_NAME("Custom SceneViewExtension Post Processing CS Shader %dx%d", PassViewSize.X, PassViewSize.Y),
		ComputeShader,
		PassParameters,
		GroupCount
	);

	// Copy the output texture back to SceneColor
	// Returning the new texture as ScreenPassTexture doesn't work, so this is pretty fast alternative
	// Also with f.ex 'PrePostProcessPass_RenderThread' you get only input and something similar needs to be implemented then
	AddCopyTexturePass(GraphBuilder, OutputTexture, SceneColor.Texture);

	return SceneColor;
}

bool FDatamoshShader::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
}

void FDatamoshShader::ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters,
                                                   FShaderCompilerEnvironment& OutEnvironment)
{
	OutEnvironment.SetDefine(TEXT("THREADS_X"), 8);
	OutEnvironment.SetDefine(TEXT("THREADS_Y"), 8);
	OutEnvironment.SetDefine(TEXT("THREADS_Z"), 1);
}
