// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UserSettings/OptionBindingBase.h"
#include "VolumetricFogBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Volumetric Fog Quality")
struct TORORUNTIME_API FVolumetricFogQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FVolumetricFogQualityBinding()
	{
		Name = INVTEXT("Volumetric Fog Quality");
		Tooltip = INVTEXT("Quality and resolution of volumetric fog and light rays. Higher values may look better at the cost of additional rendering time.");
		Impact = EUserOptionImpact::High;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetVolumetricFogQuality(); }
	virtual void SetValue(const uint8 InValue) override { GetSettings()->SetVolumetricFogQuality(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Temporal Reprojection")
struct TORORUNTIME_API FTemporalReprojectionBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FTemporalReprojectionBinding()
	{
		Name = INVTEXT("Temporal Reprojection");
		Tooltip = INVTEXT("Uses data from previous frames to efficiently calculate higher quality volumetric fog.\nNOTE: Can cause visual artifacts as a result. If turned off, you may need to increase the quality to compensate.");
		Impact = EUserOptionImpact::Medium;
	}

	virtual bool GetValue() const override { return GetSettings()->GetTemporalReprojection(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetTemporalReprojection(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Screen Space Fog Scattering")
struct TORORUNTIME_API FSSFogScatteringBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FSSFogScatteringBinding()
	{
		Name = INVTEXT("Screen Space Fog Scattering");
		Tooltip = INVTEXT("Simulate light scattering in Exponential Height Fog.");
		Impact = EUserOptionImpact::Low;
	}

	virtual bool GetValue() const override { return GetSettings()->GetSSFogScattering(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetSSFogScattering(InValue); }
};