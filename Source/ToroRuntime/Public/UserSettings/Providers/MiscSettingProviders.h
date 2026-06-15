// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UserSettings/Providers/UserSettingsProvider.h"
#include "MiscSettingProviders.generated.h"

USTRUCT(BlueprintType, DisplayName = "[Misc] Brightness")
struct TORORUNTIME_API FToroUSP_MiscBrightness : public FUserSettingsProvider_Float
{
	GENERATED_BODY()

public:

	FToroUSP_MiscBrightness();

	virtual float GetValue() const override;
	virtual void SetValue(const float InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Misc] Motion Blur")
struct TORORUNTIME_API FToroUSP_MiscMotionBlur : public FUserSettingsProvider_IntSwap
{
	GENERATED_BODY()

public:

	FToroUSP_MiscMotionBlur();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Misc] Lumen Mode")
struct TORORUNTIME_API FToroUSP_MiscLumenMode : public FUserSettingsProvider_IntSwap
{
	GENERATED_BODY()

public:

	FToroUSP_MiscLumenMode();

	virtual uint8 GetValue() const override;
	virtual void SetValue(const uint8 InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Misc] Sound Volume")
struct TORORUNTIME_API FToroUSP_MiscSoundVol : public FUserSettingsProvider_Float
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
		TObjectPtr<USoundClass> Channel;

public:

	FToroUSP_MiscSoundVol();

	virtual float GetValue() const override;
	virtual void SetValue(const float InValue) override;
};