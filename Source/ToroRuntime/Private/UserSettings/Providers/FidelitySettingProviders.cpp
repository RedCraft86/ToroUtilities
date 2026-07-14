// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Providers/FidelitySettingProviders.h"
#include "UserSettings/ImageFidelityAPI.h"

FToroUSP_FidelityMode::FToroUSP_FidelityMode()
{
	DisplayName = INVTEXT("Image Fidelity");
	Performance = EUserSettingImpact::Varies;
	bUpdateDynamically = true;

	DefaultChoices = {};
	DefaultOption = ImageFidelityAPI::ModeToNameMap[EImageFidelityMode::None].ToString();

	FTextBuilder Builder;
	{
		Builder.AppendLine(INVTEXT("Anti-Aliasing and/or Upscaler to use.\n"));
		Builder.AppendLine(INVTEXT("None: No anti-aliasing. Fastest, but edges may appear jagged."));
		Builder.AppendLine(INVTEXT("FXAA: Fast Approximate Anti-Aliasing. Lightweight AA with minimal performance cost."));
		Builder.AppendLine(INVTEXT("SMAA: Subpixel Morphological Anti-Aliasing. Higher quality than FXAA with a small additional cost."));
		Builder.AppendLine(INVTEXT("CMAA2: Conservative Morphological Anti-Aliasing 2. Focused on providing good results while retaining the sharpness unlike TAA's blurring."));
		Builder.AppendLine(INVTEXT("TAA: Temporal Anti-Aliasing. Stable image quality by blending frames over time. May cause ghosting on fast motion and is generally blurry."));
		Builder.AppendLine(INVTEXT("TSR: Temporal Super Resolution. Unreal Engine 5's upscaler. Recommended only on high end hardware. May cause ghosting on fast motion."));
		Builder.AppendLine(INVTEXT("FSR: AMD FidelityFX Super Resolution 4. AI-based upscaling on AMD RDNA 4 or traditional upscaling on other hardware."));
		Builder.AppendLine(INVTEXT("XeSS: Intel Xe Super Sampling. AI-based upscaling optimised for Intel Arc GPUs but compatible with others."));
		Builder.AppendLine(INVTEXT("DLSS: NVIDIA Deep Learning Super Sampling. AI-based upscaling. Requires an NVIDIA RTX GPU."));
	}
	Description = Builder.ToText();
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
