// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UserSettingsProvider.h"
#include "ResolutionSettingProviders.generated.h"

USTRUCT(BlueprintType, DisplayName = "[Resolution] Scale Percentage")
struct TORORUNTIME_API FToroUSP_ResolutionScale : public FUserSettingsProvider_Float
{
	GENERATED_BODY()

public:

	FToroUSP_ResolutionScale();

	virtual float GetValue() const override;
	virtual void SetValue(const float InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Resolution] Value & Mode")
struct TORORUNTIME_API FToroUSP_ResolutionValue final : public FUserSettingsProvider_String
{
	GENERATED_BODY()

public:

	FToroUSP_ResolutionValue();

	virtual const TArray<FString>& GetOptions() override;

	virtual FString GetValue() const override;
	virtual void SetValue(const FString& InValue) override;

private:

	static inline const TMap<EWindowMode::Type, FString> SUFFIXES = {
		{ EWindowMode::Fullscreen,			TEXT("(Fullscreen)")	},
		{ EWindowMode::WindowedFullscreen,	TEXT("(Borderless)")	},
		{ EWindowMode::Windowed,				TEXT("(Windowed)")		}
	};

	static FString BuildOption(const FIntPoint& Resolution, EWindowMode::Type WindowMode);
	static void SplitOption(const FString& Value, FIntPoint& Resolution, EWindowMode::Type& WindowMode);
};
