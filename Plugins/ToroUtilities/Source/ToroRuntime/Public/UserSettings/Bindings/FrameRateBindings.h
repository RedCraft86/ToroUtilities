// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/ImageFidelity.h"
#include "UserSettings/OptionBindingBase.h"
#include "FrameRateBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Show FPS")
struct TORORUNTIME_API FShowFPSBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FShowFPSBinding()
	{
		Name = INVTEXT("Show FPS");
		Tooltip = INVTEXT("Show the current Frame Rate and Frame Time on the upper right side of the screen.");
	}

	virtual bool GetValue() const override { return GetSettings()->GetShowFPS(); }
	virtual void SetValue(const bool InValue) override { GetSettings()->SetShowFPS(InValue); }
};

USTRUCT(BlueprintType, DisplayName = "Frame Rate Limit")
struct TORORUNTIME_API FFrameRateLimitBinding : public FSwapperOptionBinding
{
	GENERATED_BODY()

	FFrameRateLimitBinding()
	{
		Name = INVTEXT("Frame Rate Limit");
		Tooltip = INVTEXT("Cap/Uncap the maximum Frame Rate. V-Sync may override.");
		Options = {"30", "60", "120", "Unlimited"};
		Impact = EUserOptionImpact::Varies;
	}

	virtual uint8 GetValue() const override
	{
		const float FPS = GetSettings()->GetFrameRateLimit();
		if (FPS < 1.0f) return 3;
		if (FPS < 31.0f) return 0;
		if (FPS < 61.0f) return 1;
		if (FPS < 121.0f) return 2;
		return 3;
	}

	virtual void SetValue(const uint8 InValue) override
	{
		float FPS;
		switch (InValue)
		{
			case 0:	 FPS = 30.0f;  break;
			case 1:	 FPS = 60.0f;  break;
			case 2:	 FPS = 120.0f; break;
			default: FPS = 500.0f; break;
		}
		GetSettings()->SetFrameRateLimit(FPS);
	}
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || GetSettings()->IsVSyncEnabled();
	}
};

USTRUCT(BlueprintType, DisplayName = "V-Sync")
struct TORORUNTIME_API FVSyncBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FVSyncBinding()
	{
		Name = INVTEXT("V-Sync");
		Tooltip = INVTEXT("Synchronize the Frame Rate with the monitor's Refresh Rate. Implicitly sets a Frame Rate cap.\nAuto disabled if any frame generation solutions are on.");
		Impact = EUserOptionImpact::Varies;
	}

	virtual bool GetValue() const override { return GetSettings()->IsVSyncEnabled(); }
	virtual void SetValue(const bool InValue) override
	{
		GetSettings()->SetVSyncEnabled(InValue);
		if (InValue)
		{
			GetSettings()->SetFrameRateLimit(0.0f);
		}
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
	virtual bool ShouldHide() const override
	{
		return Super::ShouldHide() || ImageFidelity::IsUsingAnyFrameGen();
	}
};