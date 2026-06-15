// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Providers/ScalabilitySettingProviders.h"

FToroUSP_ScalabilityBase::FToroUSP_ScalabilityBase()
{
	Performance = EUserSettingImpact::Medium;
	bUpdateDynamically = true;

	DefaultOption = 2; // High
	OptionNames = {
		TEXT("Low"), 
		TEXT("Medium"), 
		TEXT("High"), 
		TEXT("Epic"), 
		TEXT("Cinematic")
	};
}

FToroUSP_ScalabilityOverall::FToroUSP_ScalabilityOverall()
{
	DisplayName = INVTEXT("Overall Scalability");
	Description = INVTEXT("Adjusts all scalability settings at once.");
	Performance = EUserSettingImpact::High;

	DefaultOption = 3; // High
	OptionNames = {
		TEXT("Custom")
		TEXT("Low"), 
		TEXT("Medium"), 
		TEXT("High"), 
		TEXT("Epic"), 
		TEXT("Cinematic")
	};
}

uint8 FToroUSP_ScalabilityOverall::GetValue() const
{
	return UserSettings->GetOverallScalabilityLevel() + 1; // -1 is custom, but we need it to be 0
}

void FToroUSP_ScalabilityOverall::SetValue(const uint8 InValue)
{
	// Undo the adjustment made in GetValue and make "Custom" not a valid option
	if (const int Value = static_cast<int>(InValue) - 1; Value >= 0)
	{
		UserSettings->SetOverallScalabilityLevel(Value);
		UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
	}
}

FToroUSP_ScalabilityViewDist::FToroUSP_ScalabilityViewDist()
{
	DisplayName = INVTEXT("View Distance Quality");
	Description = INVTEXT("Controls how far away objects and details are rendered.");
}

uint8 FToroUSP_ScalabilityViewDist::GetValue() const
{
	return UserSettings->GetViewDistanceQuality();
}

void FToroUSP_ScalabilityViewDist::SetValue(const uint8 InValue)
{
	UserSettings->SetViewDistanceQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityAntiAlias::FToroUSP_ScalabilityAntiAlias()
{
	DisplayName = INVTEXT("Anti Aliasing Quality");
	Description = INVTEXT("Smooths jagged edges on objects and improves overall image stability.\nUse Image Fidelity Mode for the actual method.");
}

uint8 FToroUSP_ScalabilityAntiAlias::GetValue() const
{
	return UserSettings->GetAntiAliasingQuality();
}

void FToroUSP_ScalabilityAntiAlias::SetValue(const uint8 InValue)
{
	UserSettings->SetAntiAliasingQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityShadow::FToroUSP_ScalabilityShadow()
{
	DisplayName = INVTEXT("Shadow Quality");
	Description = INVTEXT("Adjusts the resolution, draw distance, and detail of shadows and volumetric rays.");
	Performance = EUserSettingImpact::High;
}

uint8 FToroUSP_ScalabilityShadow::GetValue() const
{
	return UserSettings->GetShadowQuality();
}

void FToroUSP_ScalabilityShadow::SetValue(const uint8 InValue)
{
	UserSettings->SetShadowQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityGI::FToroUSP_ScalabilityGI()
{
	DisplayName = INVTEXT("Global Illumination Quality");
	Description = INVTEXT("Controls the accuracy of bounced lighting and indirect illumination.");
	Performance = EUserSettingImpact::High;
}

uint8 FToroUSP_ScalabilityGI::GetValue() const
{
	return UserSettings->GetGlobalIlluminationQuality();
}

void FToroUSP_ScalabilityGI::SetValue(const uint8 InValue)
{
	UserSettings->SetGlobalIlluminationQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityReflection::FToroUSP_ScalabilityReflection()
{
	DisplayName = INVTEXT("Reflection Quality");
	Description = INVTEXT("Adjusts the detail and resolution of reflections on surfaces.");
	Performance = EUserSettingImpact::High;
}

uint8 FToroUSP_ScalabilityReflection::GetValue() const
{
	return UserSettings->GetReflectionQuality();
}

void FToroUSP_ScalabilityReflection::SetValue(const uint8 InValue)
{
	UserSettings->SetReflectionQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityPostProc::FToroUSP_ScalabilityPostProc()
{
	DisplayName = INVTEXT("Post Processing Quality");
	Description = INVTEXT("Controls effects such as bloom, depth of field, lens flares, and more.\nLow is not recommended as it may break crucial effects.");

	OptionNames = {
		TEXT("Low (Not Recommended)"), 
		TEXT("Medium"), 
		TEXT("High"), 
		TEXT("Epic"), 
		TEXT("Cinematic")
	};
}

uint8 FToroUSP_ScalabilityPostProc::GetValue() const
{
	return UserSettings->GetPostProcessingQuality();
}

void FToroUSP_ScalabilityPostProc::SetValue(const uint8 InValue)
{
	UserSettings->SetPostProcessingQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityTexture::FToroUSP_ScalabilityTexture()
{
	DisplayName = INVTEXT("Texture Quality");
	Description = INVTEXT("Adjusts the resolution of textures used for objects and surfaces.\nHigher values may use more VRAM.");
}

uint8 FToroUSP_ScalabilityTexture::GetValue() const
{
	return UserSettings->GetTextureQuality();
}

void FToroUSP_ScalabilityTexture::SetValue(const uint8 InValue)
{
	UserSettings->SetTextureQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityEffect::FToroUSP_ScalabilityEffect()
{
	DisplayName = INVTEXT("Visual Effect Quality");
	Description = INVTEXT("Controls the detail and complexity of particle and other visual effects.");
	Performance = EUserSettingImpact::High;
}

uint8 FToroUSP_ScalabilityEffect::GetValue() const
{
	return UserSettings->GetVisualEffectQuality();
}

void FToroUSP_ScalabilityEffect::SetValue(const uint8 InValue)
{
	UserSettings->SetVisualEffectQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityFoliage::FToroUSP_ScalabilityFoliage()
{
	DisplayName = INVTEXT("Foliage Quality");
	Description = INVTEXT("Adjusts the density, variety, and draw distance of grass, trees, and plants.");
	Performance = EUserSettingImpact::High;
}

uint8 FToroUSP_ScalabilityFoliage::GetValue() const
{
	return UserSettings->GetFoliageQuality();
}

void FToroUSP_ScalabilityFoliage::SetValue(const uint8 InValue)
{
	UserSettings->SetFoliageQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityShading::FToroUSP_ScalabilityShading()
{
	DisplayName = INVTEXT("Shading Quality");
	Description = INVTEXT("Controls shader complexity, material quality, and surface detail.");
}

uint8 FToroUSP_ScalabilityShading::GetValue() const
{
	return UserSettings->GetShadingQuality();
}

void FToroUSP_ScalabilityShading::SetValue(const uint8 InValue)
{
	UserSettings->SetShadingQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

FToroUSP_ScalabilityLandscape::FToroUSP_ScalabilityLandscape()
{
	DisplayName = INVTEXT("Landscape Quality");
	Description = INVTEXT("Adjusts the detail and tessellation of landscapes and terrain.");
}

uint8 FToroUSP_ScalabilityLandscape::GetValue() const
{
	return UserSettings->GetLandscapeQuality();
}

void FToroUSP_ScalabilityLandscape::SetValue(const uint8 InValue)
{
	UserSettings->SetLandscapeQuality(InValue);
	UserSettings->BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}
