// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Components/TextBlock.h"
#include "DataTypes/CachedGetter.h"
#include "UserSettings/ToroGameUserSettings.h"
#include "FrameRateBox.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UFrameRateBox final : public UTextBlock
{
	GENERATED_BODY()

public:

	UFrameRateBox();

private:

	bool bInit;
	bool bShowingFPS;
	FNumberFormattingOptions FPSOptions;
	FNumberFormattingOptions MSOptions;

	TCachedGetter<UToroGameUserSettings> UserSettings {[]()
	{
		return UToroGameUserSettings::Get();
	}};

	EActiveTimerReturnType Update(double CurrentTime, float DeltaTime);
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
