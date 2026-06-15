// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Providers/FidelitySettingProviders.h"
#include "UserSettings/ImageFidelityAPI.h"

FToroUSP_FidelityMode::FToroUSP_FidelityMode()
{
	DisplayName = INVTEXT("Image Fidelity");
	Description = INVTEXT("Anti-Aliasing and/or Upscaler to use.");
	Performance = EUserSettingImpact::Varies;
	bUpdateDynamically = true;

	DefaultChoices = {};
	DefaultOption = ImageFidelityAPI::ModeToNameMap[EImageFidelityMode::None].ToString();

	OptionTooltips = {
		{TEXT("None"),	INVTEXT("No anti-aliasing. Fastest, but edges may appear jagged.")},
		{TEXT("FXAA"),	INVTEXT("Fast Approximate Anti-Aliasing. Lightweight AA with minimal performance cost.")},
		{TEXT("SMAA"),	INVTEXT("Subpixel Morphological Anti-Aliasing. Higher quality than FXAA with a small additional cost.")},
		{TEXT("CMAA2"),	INVTEXT("Conservative Morphological Anti-Aliasing 2. Focused on providing good results while retaining the sharpness unlike TAA's blurring.")},
		{TEXT("TAA"),	INVTEXT("Temporal Anti-Aliasing. Stable image quality by blending frames over time. May cause ghosting on fast motion and is generally blurry.")},
		{TEXT("TSR"),	INVTEXT("Temporal Super Resolution. Unreal Engine 5's upscaler. Recommended only on high end hardware. May cause ghosting on fast motion.")},
		{TEXT("FSR"),	INVTEXT("AMD FidelityFX Super Resolution 4. AI-based upscaling on AMD RDNA 4; traditional upscaling on other hardware.")},
		{TEXT("XeSS"),	INVTEXT("Intel Xe Super Sampling. AI-based upscaling optimised for Intel Arc GPUs, compatible with others.")},
		{TEXT("DLSS"),	INVTEXT("NVIDIA Deep Learning Super Sampling. AI-based upscaling. Requires an NVIDIA RTX GPU.")}
	};
}

const TArray<FString>& FToroUSP_FidelityMode::GetOptions()
{
	if (!DefaultChoices.IsEmpty())
	{
		return DefaultChoices;
	}

	for (const EImageFidelityMode Mode : TEnumRange<EImageFidelityMode>())
	{
		if (ImageFidelityAPI::IsSupportedMode(Mode))
		{
			DefaultChoices.Add(ImageFidelityAPI::ModeToNameMap[Mode].ToString());
		}
	}

	return DefaultChoices;
}

FString FToroUSP_FidelityMode::GetValue() const
{
	return ImageFidelityAPI::ModeToNameMap[UserSettings->GetImageFidelityMode()].ToString();
}

void FToroUSP_FidelityMode::SetValue(const FString& InValue)
{
	const EImageFidelityMode* ValuePtr = ImageFidelityAPI::ModeToNameMap.FindKey(FName(*InValue));
	UserSettings->SetImageFidelityMode(ValuePtr ? *ValuePtr : EImageFidelityMode::None);
}

FToroUSP_FidelityTSRPercent::FToroUSP_FidelityTSRPercent()
{
	DisplayName = INVTEXT("[TSR] Screen Percentage");
	Description = INVTEXT("To render in lower resolution and upscale for better performance.");
	bUpdateDynamically = true;

	MinValue = 10.0f;
	MaxValue = 100.0f;
	DefaultOption = 100.0f;
}

float FToroUSP_FidelityTSRPercent::GetValue() const
{
	return UserSettings->GetTSRScreenPercentage();
}

void FToroUSP_FidelityTSRPercent::SetValue(const float InValue)
{
	UserSettings->SetTSRScreenPercentage(InValue);
}
