// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "PostProcessTypes.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FPPBloomConfig
{
	GENERATED_BODY()

	/* Bloom algorithm */
	UPROPERTY(EditAnywhere, Category = Bloom)
		TEnumAsByte<EBloomMethod> Method;

	/* Multiplier for all bloom contributions >=0: off, 1(default), >1 brighter */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (ClampMin = 0.0f, UIMax = 8.0f))
		float Intensity;

	/* Minimum brightness the bloom starts having effect
	 * -1:all pixels affect bloom equally (physically correct, faster as a threshold pass is omitted), 0:all pixels affect bloom brights more, 1(default), >1 brighter
	 */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (ClampMin = -1.0f, UIMax = 8.0f, EditCondition = "Method == EBloomMethod::BM_SOG"))
		float Threshold;
	
	/* Intensity multiplier on the scatter dispersion energy of the kernel. 1.0 means exactly use the same energy as the kernel scatter dispersion. */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (ClampMin = 0.0f, UIMax = 20.0f, EditCondition = "Method == EBloomMethod::BM_FFT"))
		float ConvolutionScatterDispersion;

	/* Texture to replace default convolution bloom kernel */
	UPROPERTY(EditAnywhere, Category = Bloom, meta = (EditCondition = "Method == EBloomMethod::BM_FFT"))
		TObjectPtr<UTexture2D> ConvolutionKernel;

	FPPBloomConfig(): Method(BM_SOG), Intensity(0.675f), Threshold(-1.0f)
		, ConvolutionScatterDispersion(1.0f), ConvolutionKernel(nullptr)
	{}

	void ModifyPP(FPostProcessSettings& Settings) const;
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FPPBloomChoice final
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Bloom)
		bool bPreviewFancy = true;
#endif

	UPROPERTY(EditAnywhere, Category = Bloom)
		FPPBloomConfig Simple;

	UPROPERTY(EditAnywhere, Category = Bloom)
		FPPBloomConfig Fancy;

	FPPBloomChoice() {}
	void ApplyChoice(FPostProcessSettings& Settings, const bool bFancy) const;

private:

	const FPPBloomConfig& GetOption(const bool bFancy) const { return bFancy ? Fancy : Simple; }
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FPPMotionBlurConfig
{
	GENERATED_BODY()

	/* Strength of motion blur, 0:off */
	UPROPERTY(Interp, EditAnywhere, Category = MotionBlur, meta = (ClampMin = 0.0f, ClampMax = 1.0f))
		float Amount;

	/* Max distortion caused by motion blur, in percent of the screen width, 0:off */
	UPROPERTY(Interp, EditAnywhere, Category = MotionBlur, meta = (ClampMin = 0.0f, ClampMax = 100.0f))
		float MaxDistortion;

	FPPMotionBlurConfig(): Amount(0.5f), MaxDistortion(5.0f) {}
	FPPMotionBlurConfig(const float InAmount, const float InDistortion)
		: Amount(InAmount), MaxDistortion(InDistortion)
	{}

	void ModifyPP(FPostProcessSettings& Settings) const;
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FPPMotionBlurChoice
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = MotionBlur, meta = (ClampMin = 0, ClampMax = 3, UIMin = 0, UIMax = 3))
		uint8 PreviewQuality = 1;
#endif

	UPROPERTY(EditAnywhere, Category = MotionBlur)
		FPPMotionBlurConfig Low;

	UPROPERTY(EditAnywhere, Category = MotionBlur)
		FPPMotionBlurConfig Medium;

	UPROPERTY(EditAnywhere, Category = MotionBlur)
		FPPMotionBlurConfig High;

	FPPMotionBlurChoice() {}
	
	/* Quality 0~3 where 0: off and 3: high */
	void ApplyChoice(FPostProcessSettings& Settings, const uint8 Quality) const;

protected:

	const FPPMotionBlurConfig& GetOption(const uint8 Quality) const;
	static const inline FPPMotionBlurConfig Disabled = FPPMotionBlurConfig(0.0f, 0.0f);
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FPPLumenConfig
{
	GENERATED_BODY()

	/* Scales Lumen Scene's quality. Larger scales cause Lumen Scene to be calculated with a higher fidelity, which can be visible in reflections, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float SceneLightingQuality;

	/* Controls the size of instances that can be represented in Lumen Scene. Larger values will ensure small objects are represented, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.25f, UIMax = 4.0f))
		float SceneDetail;
	
	/* Controls how much Lumen Scene is allowed to cache lighting results to improve performance. Larger scales cause lighting changes to propagate faster, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.5f, UIMax = 4.0f))
		float SceneLightingUpdateSpeed;

	/* Scales Lumen's Final Gather quality. Larger scales reduce noise, but greatly increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float FinalGatherQuality;

	/* Controls how much Lumen Final Gather is allowed to cache lighting results to improve performance. Larger scales cause lighting changes to propagate faster, but increase GPU cost and noise. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|GI", meta = (ClampMin = 0.5f, UIMax = 4.0f))
		float FinalGatherLightingUpdateSpeed;

	/* Scales the Reflection quality. Larger scales reduce noise in reflections, but increase GPU cost. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|Reflection", meta = (ClampMin = 0.25f, UIMax = 2.0f))
		float ReflectionQuality;

	/* Sets the maximum number of recursive reflection bounces. 1 means a single reflection ray (no secondary reflections in mirrors). Currently only supported by Hardware Ray Tracing with Hit Lighting. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|Reflection", meta = (ClampMin = 1, ClampMax = 8))
		uint8 ReflectionBounces;

	/* The maximum count of refraction event to trace. When hit lighting is used, Translucent meshes will be traced when LumenMaxRefractionBounces > 0, making the reflection tracing more expensive. */
	UPROPERTY(Interp, EditAnywhere, Category = "Lumen|Reflection", meta = (ClampMin = 0, ClampMax = 64))
		uint8 RefractionBounces;

	FPPLumenConfig(): SceneLightingQuality(1.0f), SceneDetail(1.0f), SceneLightingUpdateSpeed(1.0f), FinalGatherQuality(1.0f)
		, FinalGatherLightingUpdateSpeed(1.0f), ReflectionQuality(1.0f), ReflectionBounces(1), RefractionBounces(0)
	{}

	void ModifyGI(FPostProcessSettings& Settings) const;
	void ModifyReflect(FPostProcessSettings& Settings) const;
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FPPLumenChoice final
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	/* Global Illumination Quality | Reflection Quality */
	UPROPERTY(EditAnywhere, Category = Lumen, meta = (ClampMin = 0, ClampMax = 3, UIMin = 0, UIMax = 3))
		FIntPoint PreviewLumen = FIntPoint(3);
	
	UPROPERTY(EditAnywhere, Category = Lumen)
		bool bPreviewHitReflections = true;
#endif

	UPROPERTY(EditAnywhere, Category = Lumen)
		FPPLumenConfig Low;

	UPROPERTY(EditAnywhere, Category = Lumen)
		FPPLumenConfig Medium;

	UPROPERTY(EditAnywhere, Category = Lumen)
		FPPLumenConfig High;

	FPPLumenChoice() {}

	/* Quality 0~3 where 0: off and 3: high */
	void ApplyChoice(FPostProcessSettings& Settings, const uint8 GI, const uint8 Reflect, const bool bHitReflect) const;

private:

	void ApplyGI(FPostProcessSettings& Settings, const uint8 Quality) const;
	void ApplyReflect(FPostProcessSettings& Settings, const uint8 Quality) const;
	const FPPLumenConfig& GetOption(const uint8 Quality) const;
};