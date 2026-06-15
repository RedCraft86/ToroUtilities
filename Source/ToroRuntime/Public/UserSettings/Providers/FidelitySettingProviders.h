// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UserSettingsProvider.h"
#include "FidelitySettingProviders.generated.h"

USTRUCT(BlueprintType, DisplayName = "[Fidelity] Mode")
struct TORORUNTIME_API FToroUSP_FidelityMode final : public FUserSettingsProvider_String
{
	GENERATED_BODY()

public:

	FToroUSP_FidelityMode();

	virtual const TArray<FString>& GetOptions() override;

	virtual FString GetValue() const override;
	virtual void SetValue(const FString& InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Fidelity] TSR: Screen Percentage")
struct TORORUNTIME_API FToroUSP_FidelityTSRPercent : public FUserSettingsProvider_Float
{
	GENERATED_BODY()

public:

	FToroUSP_FidelityTSRPercent();

	virtual float GetValue() const override;
	virtual void SetValue(const float InValue) override;
};