// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Providers/CameraSettingProviders.h"

FToroUSP_CameraInterp::FToroUSP_CameraInterp()
{
	DisplayName = INVTEXT("Smooth Camera");
	Description = INVTEXT("Interpolates camera turns to make them feel less sharp.");
	DefaultOption = true;
}

bool FToroUSP_CameraInterp::GetValue() const
{
	return UserSettings->GetSmoothCamera();
}

void FToroUSP_CameraInterp::SetValue(const bool bInValue)
{
	UserSettings->SetSmoothCamera(bInValue);
}

FToroUSP_CameraSensX::FToroUSP_CameraSensX()
{
	DisplayName = INVTEXT("Horizontal Sensitivity");
	Description = INVTEXT("Multiplier for sensitivity. Negative values will invert it.\nRange: [-2, -0.1)U(0.1, 2]");

	MinValue = -2.0f;
	MaxValue = +2.0f;
	DefaultOption = 1.0f;
}

float FToroUSP_CameraSensX::GetValue() const
{
	return UserSettings->GetSensitivityX();
}

void FToroUSP_CameraSensX::SetValue(const float InValue)
{
	UserSettings->SetSensitivityX(InValue);
}

FToroUSP_CameraSensY::FToroUSP_CameraSensY()
{
	DisplayName = INVTEXT("Vertical Sensitivity");
	Description = INVTEXT("Multiplier for sensitivity. Negative values will invert it.\nRange: [-2, -0.1)U(0.1, 2]");

	MinValue = -2.0f;
	MaxValue = +2.0f;
	DefaultOption = 1.0f;
}

float FToroUSP_CameraSensY::GetValue() const
{
	return UserSettings->GetSensitivityY();
}

void FToroUSP_CameraSensY::SetValue(const float InValue)
{
	UserSettings->SetSensitivityY(InValue);
}
