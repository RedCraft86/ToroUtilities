// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserSettings/ToroGameUserSettings.h"
#include "UserSettings/ImageFidelityAPI.h"
#include "Framework/ToroWorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/WorldGetter.h"
#include "Sound/SoundMix.h"
#include "Algo/Unique.h"
#include "Misc/App.h"

const TArray<FIntPoint>& UToroGameUserSettings::GetSupportedResolutions()
{
	static TArray<FIntPoint> SupportedRes;
	if (!SupportedRes.IsEmpty())
	{
		return SupportedRes;
	}

	const UToroGameUserSettings* UserSettings = Get();
	if (!UserSettings)
	{
		return SupportedRes;
	}

	static const FIntPoint DesktopResolution(UserSettings->GetDesktopResolution());
	static const FIntPoint MinResolution(1280, 720);

	static constexpr struct { int32 Num, Den; } CommonRatios[] = {
		{ 16, 9  }, 
		{ 16, 10 }, 
		{ 4,  3  }, 
		{ 21, 9  }, 
		{ 32, 9  }
	};

	// Provides lowest -> highest
	if (!UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedRes))
	{
		SupportedRes = { DesktopResolution };
	}

	// Strip duplicates from different refresh rates with same dimensions
	SupportedRes.SetNum(Algo::Unique(SupportedRes));

	SupportedRes.RemoveAll([](const FIntPoint& Resolution)
	{
		// Always allow desktop resolution
		if (Resolution == DesktopResolution)
		{
			return false;
		}

		// Filter out lower than minimum resolutions
		if (Resolution.X < MinResolution.X || Resolution.Y < MinResolution.Y)
		{
			return true;
		}

		// Check if it's a common ratio
		for (const auto& [Num, Den] : CommonRatios)
		{
			// Use cross multiplication as IsNearlyEqual can result in precision issues
			if (Resolution.X * Den == Resolution.Y * Num)
			{
				return false;
			}
		}

		return true;
	});

	// Flips to highest -> lowest
	Algo::Reverse(SupportedRes);

	return SupportedRes;
}

void UToroGameUserSettings::AutoAdjustScalability()
{
	RunHardwareBenchmark();
	ApplyHardwareBenchmarkResults();
	ApplyMotionBlur(); // Reverse the PostProcessingQuality override
	BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

void UToroGameUserSettings::SetShowFPS(const bool bShow)
{
	bShowFPS = bShow;
}

bool UToroGameUserSettings::GetShowFPS() const
{
	return bShowFPS;
}

void UToroGameUserSettings::SetSmoothCamera(const bool bSmooth)
{
	bSmoothCamera = bSmooth;
}

bool UToroGameUserSettings::GetSmoothCamera() const
{
	return bSmoothCamera;
}

void UToroGameUserSettings::SetSensitivityX(const float Value)
{
	SensitivityX = Value;
}

float UToroGameUserSettings::GetSensitivityX() const
{
	return SensitivityX;
}

void UToroGameUserSettings::SetSensitivityY(const float Value)
{
	SensitivityY = Value;
}

float UToroGameUserSettings::GetSensitivityY() const
{
	return SensitivityY;
}

void UToroGameUserSettings::SetBrightness(const uint8 Value)
{
	Brightness = FMath::Clamp<uint8>(Value, 20, 80);
	ApplyBrightness();
}

uint8 UToroGameUserSettings::GetBrightness() const
{
	return FMath::Clamp<uint8>(Brightness, 20, 80);
}

void UToroGameUserSettings::SetMotionBlurQuality(const uint8 Value)
{
	MotionBlur = FMath::Min<uint8>(Value, 3);
}

uint8 UToroGameUserSettings::GetMotionBlurQuality() const
{
	return FMath::Min<uint8>(MotionBlur, 3);
}

void UToroGameUserSettings::SetLumenMode(const ELumenUsageMode Mode)
{
	LumenMode = Mode;
}

ELumenUsageMode UToroGameUserSettings::GetLumenMode() const
{
	return LumenMode;
}

void UToroGameUserSettings::SetImageFidelityMode(const EImageFidelityMode Mode)
{
	ImageFidelity = ImageFidelityAPI::IsSupportedFidelityMode(Mode) ? Mode : EImageFidelityMode::None;
	ApplyImageFidelity();
}

EImageFidelityMode UToroGameUserSettings::GetImageFidelityMode() const
{
	return ImageFidelity;
}

void UToroGameUserSettings::SetTSRScreenPercentage(const float Percentage)
{
	TSRScreenPercentage = FMath::Clamp(Percentage, 10.0f, 100.0f);
	ApplyTSRSettings();
}

float UToroGameUserSettings::GetTSRScreenPercentage() const
{
	return TSRScreenPercentage;
}

void UToroGameUserSettings::SetSoundVolume(const USoundClass* InClass, const uint8 Value)
{
	FindOrAddSoundVolume(InClass) = Value;
}

uint8 UToroGameUserSettings::GetSoundVolume(const USoundClass* InClass)
{
	return FindOrAddSoundVolume(InClass);
}

void UToroGameUserSettings::InitializeSettings()
{
	LoadSettings(true);
	if (!bInitialized)
	{
		bInitialized = true;
		SetScreenResolution(GetDesktopResolution());
		SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	ApplySettings(false);
}

void UToroGameUserSettings::BroadcastUpdate(EUserSettingApplyType Type) const
{
	OnSettingsApplied.Broadcast(this, Type);
	OnSettingsAppliedBP.Broadcast(this, Type);
}

void UToroGameUserSettings::SetOverallScalabilityLevel(int32 Value)
{
	Super::SetOverallScalabilityLevel(Value);
	BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

void UToroGameUserSettings::ApplyNonResolutionSettings()
{
	if (!ImageFidelityAPI::SupportsVSync())
	{
		SetVSyncEnabled(false);
	}

	Super::ApplyNonResolutionSettings();

	ApplyBrightness();
	ApplyMotionBlur();
	ApplyImageFidelity();
	ApplySoundAdjustments();

	BroadcastUpdate(EUserSettingApplyType::Manual);
}

void UToroGameUserSettings::ApplyBrightness() const
{
	UToroConsoleLibrary::SetCVarFloat(TEXT("r.Color.Mid"), static_cast<float>(GetBrightness()) / 100.0f);
}

void UToroGameUserSettings::ApplyMotionBlur() const
{
	UToroConsoleLibrary::SetCVarInt(TEXT("r.MotionBlurQuality"), GetMotionBlurQuality());
}

void UToroGameUserSettings::ApplyImageFidelity()
{
	ImageFidelityAPI::SetFidelityMode(ImageFidelity);

	ApplyTSRSettings();
	// ApplyDLSS(false); TODO
	// ApplyXeSS(false);
	// ApplyFSR();

	if (!ImageFidelityAPI::SupportsVSync())
	{
		SetVSyncEnabled(false);
	}

	BroadcastUpdate(EUserSettingApplyType::UIRefresh);
}

void UToroGameUserSettings::ApplyTSRSettings() const
{
	static IConsoleVariable* CVarSP = UToroConsoleLibrary::FindCVar(TEXT("r.ScreenPercentage"));
	if (CVarSP)
	{
		CVarSP->Set(GetImageFidelityMode() == EImageFidelityMode::TSR ? GetTSRScreenPercentage() : 100.0f);
	}
}

uint8& UToroGameUserSettings::FindOrAddSoundVolume(const USoundClass* InClass)
{
	return AudioVolumes.FindOrAdd(FAudioChannelVolume(InClass->GetFName())).Volume;
}

void UToroGameUserSettings::ApplySoundAdjustments()
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const AToroWorldSettings* WS = AToroWorldSettings::Get(this);
	if (USoundMix* SoundMix = WS ? WS->DefaultBaseSoundMix : nullptr)
	{
		for (const FSoundClassAdjuster& Adjuster : SoundMix->SoundClassEffects)
		{
			if (USoundClass* SoundClass = Adjuster.SoundClassObject)
			{
				const float RawVolume = static_cast<float>(FindOrAddSoundVolume(SoundClass)) / 100.0f;
				WS->SetSoundVolume(SoundClass, FMath::Clamp(RawVolume, 0.5f, 2.0f));
			}
		}
	}
}

void UToroGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	SetVSyncEnabled(false);
	SetFrameRateLimit(60.0f);
	SetOverallScalabilityLevel(3);

	bShowFPS = false;
	bSmoothCamera = true;
	SensitivityX = 1.0f;
	SensitivityY = 1.0f;

	Brightness = 50;
	MotionBlur = 2;
	LumenMode = ELumenUsageMode::Enabled;

	ImageFidelity = EImageFidelityMode::TAA;
}

UWorld* UToroGameUserSettings::GetWorld() const
{
#if WITH_EDITOR
	return FApp::IsGame() ? FWorldGetter::Get(this) : GEngine->GetCurrentPlayWorld();
#else
	return FWorldGetter::Get(this);
#endif
}

void UToroGameUserSettings::PostInitProperties()
{
	Super::PostInitProperties();
	SetToDefaults();
}
