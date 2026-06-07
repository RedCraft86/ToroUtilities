// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UserSettingsProvider.h"
#include "FrameRateSettingProviders.generated.h"

USTRUCT(BlueprintType, DisplayName = "[Frame Rate] Display")
struct TORORUNTIME_API FToroUSP_FrameRateShow : public FUserSettingsProvider_Bool
{
	GENERATED_BODY()

public:

	FToroUSP_FrameRateShow();

	virtual bool GetValue() const override;
	virtual void SetValue(const bool bInValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Frame Rate] Max Limit")
struct TORORUNTIME_API FToroUSP_FrameRateLimit : public FUserSettingsProvider_IntSwap
{
	GENERATED_BODY()

public:

	FToroUSP_FrameRateLimit();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Frame Rate] V-Sync")
struct TORORUNTIME_API FToroUSP_FrameRateVSync : public FUserSettingsProvider_Bool
{
	GENERATED_BODY()

public:

	FToroUSP_FrameRateVSync();

	virtual bool GetValue() const override;
	virtual void SetValue(const bool bInValue) override;
	virtual bool ShouldBeEnabled() const override;
};
