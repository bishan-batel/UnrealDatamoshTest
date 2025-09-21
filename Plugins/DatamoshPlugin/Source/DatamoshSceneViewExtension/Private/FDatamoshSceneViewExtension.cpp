#include "FDatamoshSceneViewExtension.h"

#include "SceneTextures.h"
#include "SWarningOrErrorBox.h"
#include "Framework/Notifications/NotificationManager.h"
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
	InView.bCameraMotionBlur = TOptional<bool>{true};
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
	if (not bIsPassEnabled)
	{
		return;
	}
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

	const FRDGTextureRef CustomDepthStencil{Inputs.CustomDepthTexture};

	if (CustomDepthStencil == nullptr)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Datamosh effect will not work without a custom depth stencil, enable it in project settings"));
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

	// Create / Get the Velocity Fluid Field & Datamosh Canvas

	const bool DidViewportResize = PreviousViewRect.Size() != SceneColor.ViewRect.Size();

	if (DidViewportResize or VelocityFluidPooled == nullptr or DatamoshCanvasPooled == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Creating First PreviousFramePoolTexture External Texture"));

		// Save new view rect to prevent creating the texture again in further frames
		PreviousViewRect = SceneColor.ViewRect;

		const FIntPoint TextureSize{
			SceneColor.Texture->Desc.GetSize().X,
			SceneColor.Texture->Desc.GetSize().Y,
		};

		FRDGTextureDesc VelocityFluidDesc = FRDGTextureDesc::Create2D(
			TextureSize,
			PF_FloatRGBA,
			FClearValueBinding{FLinearColor{0., 0., 0.}},
			ETextureCreateFlags::None
		);

		// Create VelocityFluid texture, make it external, and place it back into the graph to use it for this frame.
		VelocityFluidPooled = GraphBuilder.ConvertToExternalTexture(
			GraphBuilder.CreateTexture(VelocityFluidDesc, TEXT("Velocity Fluid"))
		);


		// Datamosh canvas has the same exact properties as SceneColor
		DatamoshCanvasPooled = GraphBuilder.ConvertToExternalTexture(SceneColor.Texture);
	}

	if (FMath::RandRange(0.f, 1.f) < 0.002f)
	{
		DatamoshCanvasPooled = GraphBuilder.ConvertToExternalTexture(SceneColor.Texture);
	}

	FRDGTexture* const VelocityFluidRef = GraphBuilder.RegisterExternalTexture(VelocityFluidPooled);

	FRDGTexture* const DatamoshCanvasRef = GraphBuilder.RegisterExternalTexture(DatamoshCanvasPooled);

	// Setup all the descriptors to create a target texture
	FRDGTextureDesc VelocityFluidOutputDesc;
	{
		VelocityFluidOutputDesc = VelocityFluidRef->Desc;

		VelocityFluidOutputDesc.Reset();
		VelocityFluidOutputDesc.Flags |= TexCreate_UAV;
		VelocityFluidOutputDesc.Flags &= ~(TexCreate_RenderTargetable | TexCreate_FastVRAM);

		FLinearColor ClearColor{0., 0., 0., 0.};
		VelocityFluidOutputDesc.ClearValue = FClearValueBinding{ClearColor};
	}

	// Create target texture
	FRDGTextureRef VelocityFluidOutputTexture = GraphBuilder.CreateTexture(
		VelocityFluidOutputDesc,
		TEXT("Velocity Fluid Field")
	);

	// Setup all the descriptors to create a target texture
	FRDGTextureDesc OutputDesc;
	{
		OutputDesc = SceneColor.Texture->Desc;

		OutputDesc.Reset();
		OutputDesc.Flags |= TexCreate_UAV;
		OutputDesc.Flags &= ~(TexCreate_RenderTargetable | TexCreate_FastVRAM);

		FLinearColor ClearColor{0., 0., 0., 0.};
		OutputDesc.ClearValue = FClearValueBinding{ClearColor};
	}

	// Create target texture
	FRDGTextureRef OutputTexture = GraphBuilder.CreateTexture(OutputDesc, TEXT("Final SceneColor Output"));

	FRDGTextureRef DatamoshCanvasOutput = GraphBuilder.CreateTexture(OutputDesc, TEXT("DatamoshCanvasOutput"));

	// Set the shader parameters
	FDatamoshShader::FParameters* PassParameters = GraphBuilder.AllocParameters<FDatamoshShader::FParameters>();
	{
		// Input is the SceneColor from PostProcess Material Inputs
		PassParameters->OriginalSceneColor = SceneColor.Texture;

		// This frames velocity texture
		PassParameters->Velocity = SceneVelocity.Texture;

		// Fluid Velocity Field
		PassParameters->VelocityFluid = VelocityFluidRef;

		// Use ScreenPassTextureViewportParameters so we don't need to calculate these ourselves
		PassParameters->SceneColorViewport = GetScreenPassTextureViewportParameters(SceneColorViewport);

		// This frames velocity texture's viewport
		PassParameters->SceneVelocityViewport = GetScreenPassTextureViewportParameters(SceneVelocityViewport);

		PassParameters->DepthBuffer = CustomDepthStencil;

		PassParameters->DepthBufferSampler = TStaticSamplerState<SF_Point>::CreateRHI();

		PassParameters->DatamoshCanvas = DatamoshCanvasRef;

		PassParameters->DatamoshCanvasOutput = GraphBuilder.CreateUAV(FRDGTextureUAVDesc{DatamoshCanvasOutput});

		// Create UAV from Target Texture
		PassParameters->Output = GraphBuilder.CreateUAV(FRDGTextureUAVDesc{OutputTexture});

		// Create UAV from Target Texture
		PassParameters->VelocityFluidOutput = GraphBuilder.CreateUAV(FRDGTextureUAVDesc{VelocityFluidOutputTexture});
	}


	// Add Compute Pass
	{
		const FIntPoint PassViewSize = SceneColor.ViewRect.Size();

		// Set Compute Shader and execute
		const FIntVector GroupCount = FComputeShaderUtils::GetGroupCount(
			PassViewSize,
			FComputeShaderUtils::kGolden2DGroupSize
		);


		TShaderMapRef<FDatamoshShader> ComputeShader{GlobalShaderMap};

		FComputeShaderUtils::AddPass(
			GraphBuilder,
			RDG_EVENT_NAME("Datamosh SceneViewExtension Post Processing CS Shader %dx%d", PassViewSize.X,
			               PassViewSize.Y),
			ComputeShader,
			PassParameters,
			GroupCount
		);
	}

	// Shift around textures
	{
		AddCopyTexturePass(GraphBuilder, OutputTexture, SceneColor.Texture);
		AddCopyTexturePass(GraphBuilder, VelocityFluidOutputTexture, VelocityFluidRef);
		AddCopyTexturePass(GraphBuilder, DatamoshCanvasOutput, DatamoshCanvasRef);
	}


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
