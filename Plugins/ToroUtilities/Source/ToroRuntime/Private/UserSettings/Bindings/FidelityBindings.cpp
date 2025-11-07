// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/Bindings/FidelityBindings.h"

FImageFidelityBindings::FImageFidelityBindings()
{
	Options = {
		TEXT("None"),
		TEXT("FXAA"),
		TEXT("TAA"),
		TEXT("SMAA"),
		TEXT("TSR"),
		TEXT("FSR"),
		TEXT("XeSS"),
		TEXT("DLSS")
	};
	OptionTooltips = {
		{TEXT("None"),	INVTEXT("No Anti-Aliasing or Upscaling")},
		{TEXT("FXAA"),	INVTEXT("Fast Approximate Anti-Aliasing")},
		{TEXT("TAA"),		INVTEXT("Temporal Anti-Aliasing")},
		{TEXT("SMAA"),	INVTEXT("Subpixel Morphological Anti-Aliasing")},
		{TEXT("TSR"),		INVTEXT("Temporal Super-Resolution (AA + Upscaler)")},
		{TEXT("FSR"),		INVTEXT("FidelityFX Super Resolution 3 (AA + Upscaler)")},
		{TEXT("XeSS"),	INVTEXT("Intel XeSS Super Resolution (AA + Upscaler)")},
		{TEXT("DLSS"),	INVTEXT("Nvidia Deep Learning Super Sampling (AA + Upscaler)")}
	};
}

FString FImageFidelityBindings::GetValue() const
{
	const FString* Str = FidelityStringMap.FindKey(GetSettings()->GetImageFidelity());
	return Str ? *Str : TEXT("None");
}

void FImageFidelityBindings::SetValue(const FString InValue)
{
	if (const EImageFidelityMode* Mode = FidelityStringMap.Find(InValue))
	{
		GetSettings()->SetImageFidelity(*Mode);
	}
	else
	{
		GetSettings()->SetImageFidelity(EImageFidelityMode::None);
	}
}

void FImageFidelityBindings::InitBinding()
{
	Super::InitBinding();
	Options = {
		TEXT("None"),
		TEXT("FXAA"),
		TEXT("TAA"),
		// TEXT("SMAA"), TODO (5.7 Implementation)
		TEXT("TSR"),
		TEXT("FSR")
	};
	if (ImageFidelity::XeSS::SR::IsSupported())
	{
		Options.Add(TEXT("XeSS"));
	}
	if (ImageFidelity::DLSS::SR::IsSupported())
	{
		Options.Add(TEXT("DLSS"));
	}
}

FFSRQualityBinding::FFSRQualityBinding()
{
	Name = INVTEXT("FSR Quality");
	Tooltip = INVTEXT("Quality preset for AMD FidelityFX Super Resolution upscaling.");
	Impact = EUserOptionImpact::Varies;
	Options = {
		"Native AA",
		"Quality",
		"Balanced",
		"Performance",
		"Ultra Performance"
	};
	OptionTooltips = {
		{"Native AA", INVTEXT("100% internal resolution, no upscaling")},
		{"Quality", INVTEXT("67% internal resolution, minor quality loss")},
		{"Balanced", INVTEXT("59% internal resolution, balanced quality/performance")},
		{"Performance", INVTEXT("50% internal resolution, noticeable quality loss")},
		{"Ultra Performance", INVTEXT("33% internal resolution, significant quality loss")}
	};
}

FFSRSharpnessBinding::FFSRSharpnessBinding()
{
	Name = INVTEXT("FSR Sharpness");
	Tooltip = INVTEXT("Sharpening intensity applied during FSR upscaling. Higher values increase edge definition but may introduce artifacts.");
	bConstantUpdate = true;
}

FFSRFrameGenBinding::FFSRFrameGenBinding()
{
	Name = INVTEXT("FSR Frame Generation");
	Tooltip = INVTEXT("Interpolates additional frames to increase framerate. Requires FSR to be enabled. May introduce latency and artifacts.");
	Impact = EUserOptionImpact::Varies;
}

FXeSSQualityBinding::FXeSSQualityBinding()
{
	Name = INVTEXT("XeSS Quality");
	Tooltip = INVTEXT("Quality preset for Intel XeSS upscaling.");
	Impact = EUserOptionImpact::Varies;
	Options = {
		"Native AA",
		"Ultra Quality Plus",
		"Ultra Quality",
		"Quality",
		"Balanced",
		"Performance",
		"Ultra Performance"
	};
	OptionTooltips = {
		{"Native AA", INVTEXT("100% internal resolution, no upscaling")},
		{"Ultra Quality Plus", INVTEXT("77% internal resolution, minimal quality loss")},
		{"Ultra Quality", INVTEXT("67% internal resolution, minor quality loss")},
		{"Quality", INVTEXT("59% internal resolution, balanced quality/performance")},
		{"Balanced", INVTEXT("50% internal resolution, moderate quality loss")},
		{"Performance", INVTEXT("44% internal resolution, noticeable quality loss")},
		{"Ultra Performance", INVTEXT("33% internal resolution, significant quality loss")}
	};
}

FXeSSFrameGenBinding::FXeSSFrameGenBinding()
{
	Name = INVTEXT("XeSS Frame Generation");
	Tooltip = INVTEXT("Generates additional frames to increase framerate. Requires XeSS and compatible Intel GPU. May introduce latency.");
	Impact = EUserOptionImpact::Varies;
}

FXeSSLowLatencyBinding::FXeSSLowLatencyBinding()
{
	Name = INVTEXT("XeSS Low Latency");
	Tooltip = INVTEXT("Reduces input latency at the cost of slightly higher CPU usage. Requires XeSS and compatible Intel GPU.");
	Impact = EUserOptionImpact::Varies;
}

FDLSSQualityBinding::FDLSSQualityBinding()
{
	Name = INVTEXT("DLSS Quality");
	Tooltip = INVTEXT("Quality preset for NVIDIA Deep Learning Super Sampling.");
	Impact = EUserOptionImpact::Varies;
	Options = {
		"Auto",
		"DLAA",
		"Ultra Quality",
		"Quality",
		"Balanced",
		"Performance",
		"Ultra Performance"
	};
	OptionTooltips = {
		{"Auto", INVTEXT("Automatically selects optimal quality based on current resolution and GPU")},
		{"DLAA", INVTEXT("100% internal resolution with AI anti-aliasing, maximum image quality")},
		{"Ultra Quality", INVTEXT("77% internal resolution, near-native quality")},
		{"Quality", INVTEXT("67% internal resolution, minor quality loss")},
		{"Balanced", INVTEXT("58% internal resolution, balanced quality/performance")},
		{"Performance", INVTEXT("50% internal resolution, noticeable quality loss")},
		{"Ultra Performance", INVTEXT("33% internal resolution, significant quality loss")}
	};
}

uint8 FDLSSQualityBinding::GetValue() const
{
	const int32 QualityIndex = GetSettings()->GetDLSSQuality();
	if (Options.IsValidIndex(QualityIndex))
	{
		const int32 AcceptedIndex = AcceptedOptions.Find(Options[QualityIndex]);
		return AcceptedIndex != INDEX_NONE ? AcceptedIndex : 0;
	}
	return 0;
}

void FDLSSQualityBinding::SetValue(const uint8 InValue)
{
	if (AcceptedOptions.IsValidIndex(InValue))
	{
		const FName& OptionValue = AcceptedOptions[InValue];
		const int32 Index = Options.Find(OptionValue);
		if (Index != INDEX_NONE)
		{
			GetSettings()->SetDLSSQuality(Index);
		}
	}
}

void FDLSSQualityBinding::InitBinding()
{
	Super::InitBinding();
	AcceptedOptions.Empty();
	for (const FName Option : Options)
	{
		if (ImageFidelity::DLSS::SR::IsSupportedModeName(Option))
		{
			AcceptedOptions.Add(Option);
		}
	}
}

FDLSSFrameGenBinding::FDLSSFrameGenBinding()
{
	Name = INVTEXT("DLSS Frame Generation");
	Tooltip = INVTEXT("AI-generated frames to multiply framerate. Requires DLSS 3-capable GPU. Increases latency.");
	Impact = EUserOptionImpact::Varies;
	Options = {
		"Off",
		"Auto",
		"2x",
		"3x",
		"4x"
	};
}

uint8 FDLSSFrameGenBinding::GetValue() const
{
	const int32 QualityIndex = GetSettings()->GetDLSSFrameGen();
	if (Options.IsValidIndex(QualityIndex))
	{
		const int32 AcceptedIndex = AcceptedOptions.Find(Options[QualityIndex]);
		return AcceptedIndex != INDEX_NONE ? AcceptedIndex : 0;
	}
	return 0;
}

void FDLSSFrameGenBinding::SetValue(const uint8 InValue)
{
	if (AcceptedOptions.IsValidIndex(InValue))
	{
		const FName& OptionValue = AcceptedOptions[InValue];
		const int32 Index = Options.Find(OptionValue);
		if (Index != INDEX_NONE)
		{
			GetSettings()->SetDLSSFrameGen(Index);
		}
	}
}

void FDLSSFrameGenBinding::InitBinding()
{
	Super::InitBinding();
	AcceptedOptions.Empty();
	for (const FName Option : Options)
	{
		if (ImageFidelity::DLSS::FG::IsSupportedModeName(Option))
		{
			AcceptedOptions.Add(Option);
		}
	}
}

FDLSSReflexBinding::FDLSSReflexBinding()
{
	Name = INVTEXT("DLSS Reflex");
	Tooltip = INVTEXT("Reduces system latency by optimizing CPU/GPU synchronization. Automatically enabled with Frame Generation.");
	Impact = EUserOptionImpact::Varies;
	Options = {
		"Off",
		"Enabled",
		"Boost"
	};
}

uint8 FDLSSReflexBinding::GetValue() const
{
	const int32 QualityIndex = GetSettings()->GetDLSSReflex();
	if (Options.IsValidIndex(QualityIndex))
	{
		const int32 AcceptedIndex = AcceptedOptions.Find(Options[QualityIndex]);
		return AcceptedIndex != INDEX_NONE ? AcceptedIndex : 0;
	}
	return 0;
}

void FDLSSReflexBinding::SetValue(const uint8 InValue)
{
	if (AcceptedOptions.IsValidIndex(InValue))
	{
		const FName& OptionValue = AcceptedOptions[InValue];
		const int32 Index = Options.Find(OptionValue);
		if (Index != INDEX_NONE)
		{
			GetSettings()->SetDLSSReflex(Index);
		}
	}
}

void FDLSSReflexBinding::InitBinding()
{
	Super::InitBinding();
	AcceptedOptions = Options;
	if (ImageFidelity::IsUsingAnyFrameGen())
	{
		AcceptedOptions.Remove("Off");
	}
}

FDLSSRayReconstructBinding::FDLSSRayReconstructBinding()
{
	Name = INVTEXT("DLSS Ray Reconstruction");
	Tooltip = INVTEXT("AI-enhanced denoising for ray-traced lighting. Improves quality of reflections, shadows, and global illumination. Requires ray tracing.");
	Impact = EUserOptionImpact::Varies;
}
