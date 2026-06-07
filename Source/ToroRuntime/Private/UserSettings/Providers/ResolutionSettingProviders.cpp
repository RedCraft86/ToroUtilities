// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserSettings/Providers/ResolutionSettingProviders.h"
#include "Kismet/KismetSystemLibrary.h"

FToroUSP_ResolutionScale::FToroUSP_ResolutionScale()
{
	DisplayName = INVTEXT("Resolution Scale");
	Description = INVTEXT("Percentage of the selected resolution to actually render.");
	bUpdateDynamically = true;

	MinValue = 10.0f;
	MaxValue = 100.0f;
	DefaultOption = 100.0f;
}

float FToroUSP_ResolutionScale::GetValue() const
{
	return UserSettings->GetResolutionScaleNormalized() * 100.0f;
}

void FToroUSP_ResolutionScale::SetValue(const float InValue)
{
	UserSettings->SetResolutionScaleNormalized(InValue / 100.0f);
}

FToroUSP_ResolutionValue::FToroUSP_ResolutionValue()
{
	DisplayName = INVTEXT("Resolution");
	Description = INVTEXT("Window size and fullscreen mode of the game.");
	Performance = EUserSettingImpact::Varies;
	bUpdateDynamically = true;

	DefaultChoices = {};
	DefaultOption = BuildOption(FIntPoint(0, 0), EWindowMode::Windowed);
	GetOptions(); // Initialize resolution choices
}

const TArray<FString>& FToroUSP_ResolutionValue::GetOptions()
{
	if (!DefaultChoices.IsEmpty() || !UserSettings.Get())
	{
		return DefaultChoices;
	}

	const FIntPoint DesktopRes(UserSettings->GetDesktopResolution());
	const TArray<FIntPoint>& SupportedRes = UserSettings->GetSupportedResolutions();

	for (const FIntPoint& Resolution : SupportedRes)
	{
		// Fullscreen always available
		DefaultChoices.Add(BuildOption(Resolution, EWindowMode::Fullscreen));
		if (Resolution.X == DesktopRes.X && Resolution.Y == DesktopRes.Y)
		{
			// Matches desktop -> fullscreen or borderless
			DefaultOption = BuildOption(Resolution, EWindowMode::WindowedFullscreen);
			DefaultChoices.Add(DefaultOption);
		}
		else if (Resolution.X <= DesktopRes.X && Resolution.Y <= DesktopRes.Y)
		{
			// Fits desktop but not matching -> fullscreen or windowed
			DefaultChoices.Add(BuildOption(Resolution, EWindowMode::Windowed));
		}
	}

	OptionTooltips.Add(BuildOption(DesktopRes, EWindowMode::Fullscreen), INVTEXT("Default Fullscreen"));
	OptionTooltips.Add(BuildOption(DesktopRes, EWindowMode::WindowedFullscreen), INVTEXT("Default Borderless"));

	return DefaultChoices;
}

FString FToroUSP_ResolutionValue::GetValue() const
{
	return BuildOption(UserSettings->GetScreenResolution(), UserSettings->GetFullscreenMode());
}

void FToroUSP_ResolutionValue::SetValue(const FString& InValue)
{
	FIntPoint Resolution;
	EWindowMode::Type WindowMode;
	SplitOption(InValue, Resolution, WindowMode);
	UserSettings->SetScreenResolution(Resolution);
	UserSettings->SetFullscreenMode(WindowMode);
	UserSettings->ApplyResolutionSettings(false);
	// TODO confirmation
}

FString FToroUSP_ResolutionValue::BuildOption(const FIntPoint& Resolution, EWindowMode::Type WindowMode)
{
	return FString::Printf(TEXT("%dx%d %s"), Resolution.X, Resolution.Y, *SUFFIXES.FindRef(WindowMode));
}

void FToroUSP_ResolutionValue::SplitOption(const FString& Value, FIntPoint& Resolution, EWindowMode::Type& WindowMode)
{
	FString ResolutionStr, ModeSuffix;
	Value.Split(TEXT(" "), &ResolutionStr, &ModeSuffix);

	FString ResX, ResY;
	ResolutionStr.Split(TEXT("x"), &ResX, &ResY);
	Resolution.X = FMath::Max(FCString::Atoi(*ResX), 1280);
	Resolution.Y = FMath::Max(FCString::Atoi(*ResY), 720);

	const EWindowMode::Type* WindowModePtr = SUFFIXES.FindKey(ModeSuffix);
	WindowMode = WindowModePtr ? *WindowModePtr : EWindowMode::Windowed;
}