// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/OptionBindingBase.h"
#include "LumenBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Lumen Global Illumination")
struct TORORUNTIME_API FLumenGIBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FLumenGIBinding()
	{
		Name = INVTEXT("Lumen Global Illumination");
		Tooltip = INVTEXT("Off, or Intensity of Lumen Global Illumination.");
		Impact = EUserOptionImpact::High;
		Options = OLMH;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetLumenGI(); }
	virtual void SetValue(const uint8 InValue) override { GetSettings()->SetLumenGI(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Lumen Reflection")
struct TORORUNTIME_API FLumenReflectionBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FLumenReflectionBinding()
	{
		Name = INVTEXT("Lumen Reflection");
		Tooltip = INVTEXT("Off, or Intensity of Lumen Reflections.");
		Impact = EUserOptionImpact::Medium;
		Options = OLMH;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetLumenReflection(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetLumenReflection(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Hit Lighting Reflections")
struct TORORUNTIME_API FHitLightingBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FHitLightingBinding()
	{
		Name = INVTEXT("Hit Lighting (Reflections)");
		Tooltip = INVTEXT("Use Hit Lighting rather than Surface Cache for Lumen Reflections.");
		Impact = EUserOptionImpact::High;
	}

	virtual bool GetValue() const override { return GetSettings()->GetHitLighting(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetHitLighting(InValue); }
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || GetSettings()->GetLumenReflection() == 0;
	}
};