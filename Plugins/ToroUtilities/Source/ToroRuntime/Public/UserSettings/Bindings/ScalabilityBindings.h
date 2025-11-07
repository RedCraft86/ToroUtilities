// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/OptionBindingBase.h"
#include "ScalabilityBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Overall Quality")
struct TORORUNTIME_API FOverallQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FOverallQualityBinding()
	{
		Name = INVTEXT("Overall Quality");
		Tooltip = INVTEXT("Adjust the quality for all scalability values.");
		Options = {"Custom", "Low", "Medium", "High", "Epic", "Cinematic"};
		Impact = EUserOptionImpact::High;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetOverallQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetOverallQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "View Distance Quality")
struct TORORUNTIME_API FViewDistanceQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FViewDistanceQualityBinding()
	{
		Name = INVTEXT("View Distance Quality");
		Tooltip = INVTEXT("Controls how far away objects and details are rendered.");
		Impact = EUserOptionImpact::High;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetViewDistanceQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetViewDistanceQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Anti-Aliasing Quality")
struct TORORUNTIME_API FAntiAliasingQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FAntiAliasingQualityBinding()
	{
		Name = INVTEXT("Anti-Aliasing Quality");
		Tooltip = INVTEXT("Smooths jagged edges on objects and improves overall image stability.");
		Impact = EUserOptionImpact::Medium;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetAntiAliasingQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetAntiAliasingQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Shadow Quality")
struct TORORUNTIME_API FShadowQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FShadowQualityBinding()
	{
		Name = INVTEXT("Shadow Quality");
		Tooltip = INVTEXT("Adjusts the resolution, draw distance, and detail of shadows.");
		Impact = EUserOptionImpact::High;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetShadowQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetShadowQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Global Illumination Quality")
struct TORORUNTIME_API FGlobalIlluminationQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FGlobalIlluminationQualityBinding()
	{
		Name = INVTEXT("Global Illumination Quality");
		Tooltip = INVTEXT("Controls the accuracy of bounced lighting and indirect illumination.");
		Impact = EUserOptionImpact::High;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetGlobalIlluminationQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetGlobalIlluminationQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Reflection Quality")
struct TORORUNTIME_API FReflectionQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FReflectionQualityBinding()
	{
		Name = INVTEXT("Reflection Quality");
		Tooltip = INVTEXT("Adjusts the detail and resolution of reflections on surfaces.");
		Impact = EUserOptionImpact::High;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetReflectionQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetReflectionQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Post Processing Quality")
struct TORORUNTIME_API FPostProcessQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FPostProcessQualityBinding()
	{
		Name = INVTEXT("Post Processing Quality");
		Tooltip = INVTEXT("Controls effects such as bloom, depth of field, lens flares, and more.");
		Impact = EUserOptionImpact::Medium;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetPostProcessingQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetPostProcessingQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Texture Quality")
struct TORORUNTIME_API FTextureQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FTextureQualityBinding()
	{
		Name = INVTEXT("Texture Quality");
		Tooltip = INVTEXT("Adjusts the resolution of textures used for objects and surfaces.");
		Impact = EUserOptionImpact::Medium;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetTextureQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetTextureQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Effects Quality")
struct TORORUNTIME_API FEffectsQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FEffectsQualityBinding()
	{
		Name = INVTEXT("Effects Quality");
		Tooltip = INVTEXT("Controls the detail and complexity of particle and other visual effects.");
		Impact = EUserOptionImpact::High;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetVisualEffectQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetVisualEffectQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Foliage Quality")
struct TORORUNTIME_API FFoliageQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FFoliageQualityBinding()
	{
		Name = INVTEXT("Foliage Quality");
		Tooltip = INVTEXT("Adjusts the density, variety, and draw distance of grass, trees, and plants.");
		Impact = EUserOptionImpact::Medium;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetFoliageQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetFoliageQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "Shading Quality")
struct TORORUNTIME_API FShadingQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FShadingQualityBinding()
	{
		Name = INVTEXT("Shading Quality");
		Tooltip = INVTEXT("Controls shader complexity, material quality, and surface detail.");
		Impact = EUserOptionImpact::High;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetShadingQuality(); }
	virtual void SetValue(const uint8 InValue) override
	{
		GetSettings()->SetShadingQuality(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

/* Not Impl yet
USTRUCT(BlueprintType, DisplayName = "Landscape Quality")
struct TORORUNTIME_API FLandscapeQualityBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FLandscapeQualityBinding()
	{
		Name = INVTEXT("Landscape Quality");
		Tooltip = INVTEXT("Adjusts the detail and tessellation of landscapes and terrain.");
		Impact = EUserOptionImpact::Medium;
		Options = LMHEC;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetLandscapeQuality(); }
	virtual void SetValue(const uint8 InValue) override { GetSettings()->SetLandscapeQuality(InValue); }
};
*/