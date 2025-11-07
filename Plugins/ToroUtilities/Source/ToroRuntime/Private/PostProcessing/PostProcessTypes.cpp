// Copyright (C) RedCraft86. All Rights Reserved.

#include "PostProcessing/PostProcessTypes.h"

void FPPBloomConfig::ModifyPP(FPostProcessSettings& Settings) const
{
	Settings.bOverride_BloomMethod = true;
	Settings.BloomMethod = Method;
	
	Settings.bOverride_BloomIntensity = true;
	Settings.BloomIntensity = Intensity;
	
	Settings.bOverride_BloomThreshold = true;
	Settings.BloomThreshold = Threshold;
	
	Settings.bOverride_BloomConvolutionScatterDispersion = true;
	Settings.BloomConvolutionScatterDispersion = ConvolutionScatterDispersion;
	
	Settings.bOverride_BloomConvolutionTexture = true;
	Settings.BloomConvolutionTexture = ConvolutionKernel;
}

void FPPBloomChoice::ApplyChoice(FPostProcessSettings& Settings, const bool bFancy) const
{
	GetOption(
#if WITH_EDITORONLY_DATA
		FApp::IsGame() ? bFancy : bPreviewFancy
#else
		bFancy
#endif
	).ModifyPP(Settings);
}

void FPPMotionBlurConfig::ModifyPP(FPostProcessSettings& Settings) const
{
	Settings.bOverride_MotionBlurAmount = true;
	Settings.MotionBlurAmount = Amount;

	Settings.bOverride_MotionBlurMax = true;
	Settings.MotionBlurMax = MaxDistortion;
}

void FPPMotionBlurChoice::ApplyChoice(FPostProcessSettings& Settings, const uint8 Quality) const
{
	GetOption(
#if WITH_EDITORONLY_DATA
		FApp::IsGame() ? Quality : PreviewQuality
#else
		Quality
#endif
	).ModifyPP(Settings);
}

const FPPMotionBlurConfig& FPPMotionBlurChoice::GetOption(const uint8 Quality) const
{
	switch (Quality)
	{
		case 0: return Disabled;
		case 1:	return Low;
		case 2:	return Medium;
		default:return High;
	}
}

void FPPLumenConfig::ModifyGI(FPostProcessSettings& Settings) const
{
	Settings.bOverride_LumenSceneLightingQuality = true;
	Settings.LumenSceneLightingQuality = SceneLightingQuality;
	
	Settings.bOverride_LumenSceneDetail = true;
	Settings.LumenSceneDetail = SceneDetail;
	
	Settings.bOverride_LumenSceneLightingUpdateSpeed = true;
	Settings.LumenSceneLightingUpdateSpeed = SceneLightingUpdateSpeed;
	
	Settings.bOverride_LumenFinalGatherQuality = true;
	Settings.LumenFinalGatherQuality = FinalGatherQuality;
	
	Settings.bOverride_LumenFinalGatherLightingUpdateSpeed = true;
	Settings.LumenFinalGatherLightingUpdateSpeed = FinalGatherLightingUpdateSpeed;
}

void FPPLumenConfig::ModifyReflect(FPostProcessSettings& Settings) const
{
	Settings.bOverride_LumenReflectionQuality = true;
	Settings.LumenReflectionQuality = ReflectionQuality;
		
	Settings.bOverride_LumenMaxReflectionBounces = true;
	Settings.LumenMaxReflectionBounces = ReflectionBounces;
		
	Settings.bOverride_LumenMaxRefractionBounces = true;
	Settings.LumenMaxRefractionBounces = RefractionBounces;
}

void FPPLumenChoice::ApplyChoice(FPostProcessSettings& Settings, const uint8 GI, const uint8 Reflect, const bool bHitReflect) const
{
	Settings.bOverride_LumenRayLightingMode = true;
	Settings.LumenRayLightingMode = 
#if WITH_EDITORONLY_DATA
		(FApp::IsGame() ? bHitReflect : bPreviewHitReflections)
#else
		bHitReflect
#endif
			? ELumenRayLightingModeOverride::HitLightingForReflections
			: ELumenRayLightingModeOverride::SurfaceCache;
	
	ApplyGI(Settings,
#if WITH_EDITORONLY_DATA
		FApp::IsGame() ? GI : PreviewLumen.X
#else
		GI
#endif
	);

	ApplyReflect(Settings,
#if WITH_EDITORONLY_DATA
		FApp::IsGame() ? Reflect : PreviewLumen.Y
#else
		Reflect
#endif
	);
}

void FPPLumenChoice::ApplyGI(FPostProcessSettings& Settings, const uint8 Quality) const
{
	Settings.bOverride_DynamicGlobalIlluminationMethod = true;
	Settings.DynamicGlobalIlluminationMethod = (Quality == 0)
		? EDynamicGlobalIlluminationMethod::None
		: EDynamicGlobalIlluminationMethod::Lumen;

	GetOption(Quality).ModifyGI(Settings);
}

void FPPLumenChoice::ApplyReflect(FPostProcessSettings& Settings, const uint8 Quality) const
{
	Settings.bOverride_ReflectionMethod = true;
	Settings.ReflectionMethod = (Quality == 0)
		? EReflectionMethod::ScreenSpace
		: EReflectionMethod::Lumen;

	GetOption(Quality).ModifyReflect(Settings);
}

const FPPLumenConfig& FPPLumenChoice::GetOption(const uint8 Quality) const
{
	switch (Quality)
	{
		case 0: // Fall through
		case 1:	return Low;
		case 2:	return Medium;
		default:return High;
	}
}
