// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Providers/MiscSettingProviders.h"

FToroUSP_MiscBrightness::FToroUSP_MiscBrightness()
{
	DisplayName = INVTEXT("Brightness");
	Description = INVTEXT("The brightness of the game's mid-tones. (Does not affect UI)");

	MinValue = 20.0f;
	MaxValue = 80.0f;
	NumDecimals = 0;
	DefaultOption = 50.0f;
}

float FToroUSP_MiscBrightness::GetValue() const
{
	return UserSettings->GetBrightness();
}

void FToroUSP_MiscBrightness::SetValue(const float InValue)
{
	UserSettings->SetBrightness(static_cast<uint8>(FMath::RoundToInt32(InValue)));
}

FToroUSP_MiscMotionBlur::FToroUSP_MiscMotionBlur()
{
	DisplayName = INVTEXT("Motion Blur Quality");
	Description = INVTEXT("Off, or Quality of Motion Blur.");
	Performance = EUserSettingImpact::Low;

	DefaultOption = 2;
	OptionNames = {
		TEXT("Off"),
		TEXT("Low"),
		TEXT("Medium"),
		TEXT("High"),
		TEXT("VeryHigh")
	};
}

uint8 FToroUSP_MiscMotionBlur::GetValue() const
{
	return UserSettings->GetMotionBlurQuality();
}

void FToroUSP_MiscMotionBlur::SetValue(const uint8 InValue)
{
	UserSettings->SetMotionBlurQuality(InValue);
}

FToroUSP_MiscLumenMode::FToroUSP_MiscLumenMode()
{
	DisplayName = INVTEXT("Lumen Mode");
	Description = INVTEXT("Off, or to what extent Lumen should be enabled.");
	Performance = EUserSettingImpact::High;

	DefaultOption = 3;
	OptionNames = {
		TEXT("Disabled"),
		TEXT("Reflections Only"),
		TEXT("Lighting Only"),
		TEXT("Enabled")
	};
}

uint8 FToroUSP_MiscLumenMode::GetValue() const
{
	return static_cast<uint8>(UserSettings->GetLumenMode());
}

void FToroUSP_MiscLumenMode::SetValue(const uint8 InValue)
{
	UserSettings->SetLumenMode(static_cast<ELumenUsageMode>(FMath::Clamp<uint8>(InValue, 0, 3)));
}

FToroUSP_MiscSoundVol::FToroUSP_MiscSoundVol()
{
	DisplayName = INVTEXT("Master Volume");
	Description = INVTEXT("Overall volume of all sounds.");

	MinValue = 50.0f;
	MaxValue = 200.0f;
	NumDecimals = 0;
	DefaultOption = 100.0f;

#if WITH_EDITOR
	Channel = LoadObject<USoundClass>(nullptr, TEXT("/ToroUtilities/SoundClasses/SC_Master.SC_Master"));
#endif
}

float FToroUSP_MiscSoundVol::GetValue() const
{
	return UserSettings->GetSoundVolume(Channel);
}

void FToroUSP_MiscSoundVol::SetValue(const float InValue)
{
	UserSettings->SetSoundVolume(Channel, static_cast<uint8>(FMath::RoundToInt32(InValue)));
}
