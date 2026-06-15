// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Providers/FrameRateSettingProviders.h"
#include "UserSettings/ImageFidelityAPI.h"

FToroUSP_FrameRateShow::FToroUSP_FrameRateShow()
{
	DisplayName = INVTEXT("Show FPS");
	Description = INVTEXT("Show the current Frame Rate and Frame Time on the upper right side of the screen.");
	DefaultOption = false;
}

bool FToroUSP_FrameRateShow::GetValue() const
{
	return UserSettings->GetShowFPS();
}

void FToroUSP_FrameRateShow::SetValue(const bool bInValue)
{
	UserSettings->SetShowFPS(bInValue);
}

FToroUSP_FrameRateLimit::FToroUSP_FrameRateLimit()
{
	DisplayName = INVTEXT("Frame Rate Limit");
	Description = INVTEXT("Cap/Uncap the maximum Frame Rate. May be overridden by V-Sync.");
	Performance = EUserSettingImpact::Varies;

	DefaultOption = 1;
	OptionNames = {
		TEXT("30"),
		TEXT("60"),
		TEXT("120"),
		TEXT("Unlimited")
	};
}

uint8 FToroUSP_FrameRateLimit::GetValue() const
{
	const float Limit = UserSettings->GetFrameRateLimit();
	if (Limit < 1.0f)	return 3;
	if (Limit < 31.0f)	return 0;
	if (Limit < 61.0f)	return 1;
	if (Limit < 121.0f) return 2;
	return 3;
}

void FToroUSP_FrameRateLimit::SetValue(const uint8 InValue)
{
	float Limit;
	switch (InValue)
	{
		case 0:  Limit = 30.0f;  break;
		case 1:  Limit = 60.0f;  break;
		case 2:  Limit = 120.0f; break;
		default: Limit = 0.0f;   break;
	}
	UserSettings->SetFrameRateLimit(Limit);
}

FToroUSP_FrameRateVSync::FToroUSP_FrameRateVSync()
{
	DisplayName = INVTEXT("V-Sync");
	Description = INVTEXT("Synchronize the Frame Rate with the monitor's Refresh Rate. Implicitly sets a Frame Rate cap.\nDisabled with Frame Generation.");
	Performance = EUserSettingImpact::Varies;
	bUpdateDynamically = true;

	DefaultOption = true;
}

bool FToroUSP_FrameRateVSync::GetValue() const
{
	return UserSettings->IsVSyncEnabled();
}

void FToroUSP_FrameRateVSync::SetValue(const bool bInValue)
{
	UserSettings->SetVSyncEnabled(bInValue);
}

bool FToroUSP_FrameRateVSync::ShouldBeEnabled() const
{
	return ImageFidelityAPI::SupportsVSync();
}
