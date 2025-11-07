// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/OptionBindingBase.h"
#include "MiscellaneousBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Screen Gamma")
struct TORORUNTIME_API FScreenGammaBinding : public FSliderOptionBinding
{
	GENERATED_BODY()

	FScreenGammaBinding()
	{
		Name = INVTEXT("Screen Gamma");
		Tooltip = INVTEXT("The lightness of the screen. (Affects whole screen)");
		SliderRange = {0.5f, 5.0f};
		MaxDecimals = 1;
		bConstantUpdate = true;
	}

	virtual float GetValue() const override { return GetSettings()->GetGamma(); }
	virtual void SetValue(const float InValue) override { GetSettings()->SetGamma(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Brightness")
struct TORORUNTIME_API FBrightnessBinding : public FSliderOptionBinding
{
	GENERATED_BODY()

	FBrightnessBinding()
	{
		Name = INVTEXT("Brightness");
		Tooltip = INVTEXT("The brightness of the lighting in-game. (Does not affect UI)");
		SliderRange = {0, 200};
		bConstantUpdate = true;
	}

	virtual float GetValue() const override { return GetSettings()->GetBrightness(); }
	virtual void SetValue(const float InValue) override
	{
		GetSettings()->SetBrightness(static_cast<uint8>(FMath::RoundToInt32(InValue)));
	}
};

USTRUCT(BlueprintType, DisplayName = "Fancy Bloom")
struct TORORUNTIME_API FFancyBloomBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FFancyBloomBinding()
	{
		Name = INVTEXT("Fancy Bloom");
		Tooltip = INVTEXT("Use Convolution Bloom when available.");
		Impact = EUserOptionImpact::Low;
	}

	virtual bool GetValue() const override { return GetSettings()->GetFancyBloom(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetFancyBloom(InValue); }
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

USTRUCT(BlueprintType, DisplayName = "Motion Blur")
struct TORORUNTIME_API FMotionBlurBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FMotionBlurBinding()
	{
		Name = INVTEXT("Motion Blur");
		Tooltip = INVTEXT("Off, or Intensity of Motion Blur.");
		Options = OLMH;
	}

	virtual uint8 GetValue() const override { return GetSettings()->GetMotionBlur(); }
	virtual void SetValue(const uint8 InValue) override { GetSettings()->SetMotionBlur(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Audio Volume")
struct TORORUNTIME_API FAudioVolumeBinding : public FSliderOptionBinding
{
	GENERATED_BODY()

	FAudioVolumeBinding(): SoundType(ESoundClassType::Master)
	{
		Name = INVTEXT("Audio Volume");
		Tooltip = INVTEXT("Volume of the Sound Type.");
		SliderRange = {0, 150};
		bConstantUpdate = true;
	}

	UPROPERTY(EditAnywhere, Category = Option, meta = (DisplayPriority = 2))
		ESoundClassType SoundType;

	virtual float GetValue() const override { return GetSettings()->GetAudioVolume(SoundType); }
	virtual void SetValue(const float InValue) override
	{
		GetSettings()->SetAudioVolume(SoundType, static_cast<uint8>(FMath::RoundToInt32(InValue)));
	}
};
