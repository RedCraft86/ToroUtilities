// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UserSettingsProvider.h"
#include "CameraSettingProviders.generated.h"

USTRUCT(BlueprintType, DisplayName = "[Camera] Interpolate")
struct TORORUNTIME_API FToroUSP_CameraInterp : public FUserSettingsProvider_Bool
{
	GENERATED_BODY()

public:

	FToroUSP_CameraInterp();

	virtual bool GetValue() const override;
	virtual void SetValue(const bool bInValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Camera] Sensitivity X")
struct TORORUNTIME_API FToroUSP_CameraSensX : public FUserSettingsProvider_Float
{
	GENERATED_BODY()

public:

	FToroUSP_CameraSensX();

	virtual float GetValue() const override;
	virtual void SetValue(const float InValue) override;
};

USTRUCT(BlueprintType, DisplayName = "[Camera] Sensitivity Y")
struct TORORUNTIME_API FToroUSP_CameraSensY : public FUserSettingsProvider_Float
{
	GENERATED_BODY()

public:

	FToroUSP_CameraSensY();

	virtual float GetValue() const override;
	virtual void SetValue(const float InValue) override;
};