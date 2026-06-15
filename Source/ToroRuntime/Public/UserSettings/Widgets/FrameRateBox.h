// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
