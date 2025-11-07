// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/ToroUserSettings.h"
#include "UserSettings/ImageFidelity.h"
#include "ToroRuntime.h"
#include "XeFGRHI.h"

extern ENGINE_API float GAverageFPS;
extern ENGINE_API float GAverageMS;

#define DEFINE_PROPERTY_FUNC(Type, Name, Update) \
	void UToroUserSettings::Set##Name(const Type InValue) { \
		if (Name != InValue) { \
			Name = InValue; \
			Update \
		} \
	}

#define DEFINE_PROPERTY_FUNC_CLAMPED(Type, Name, Min, Max, Update) \
	void UToroUserSettings::Set##Name(const Type InValue) { \
		if (Name != InValue) { \
			Name = FMath::Clamp(InValue, Min, Max); \
			Update \
		} \
	}

const FMouseInversion FMouseInversion::Disabled = FMouseInversion();

#define OnSettingsApply(Type) OnSettingsUpdated.Broadcast(ESettingApplyType::Type);

UToroUserSettings::UToroUserSettings(): bInitialized(false)
{
	UToroUserSettings::SetToDefaults();
}

float UToroUserSettings::GetAverageMS()
{
	return GAverageMS;
}

float UToroUserSettings::GetAverageFPS()
{
	const UToroUserSettings* UserSettings = UToroUserSettings::Get();
	const bool bUsingXeFG = UserSettings
		&& UserSettings->ImageFidelity == EImageFidelityMode::XeSS
		&& ImageFidelity::IsUsingAnyFrameGen();

	return bUsingXeFG ? GXeFGAverageFPS : GAverageFPS;
}

void UToroUserSettings::UpdateResolutions()
{
	SupportedResolutions.Empty();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(SupportedResolutions);
	FullscreenRes = SupportedResolutions.Last();
	Algo::Reverse(SupportedResolutions);
}

bool UToroUserSettings::InitializeSettings(UGameInstance* GI)
{
	if (SupportedResolutions.IsEmpty())
	{
		UpdateResolutions();
	}

	LoadSettings(true);
	bool bFirstLoad = false;
	if (!bInitialized)
	{
		GameInstance = GI;
		bInitialized = true;
		bFirstLoad = true;

		AutoAdjustScalability();
		SetResolutionPercent(100);

		SetAdjustedFullscreenMode();
		SetScreenResolution(GetFullscreenResolution());
	}

	ApplySettings(true);
	return bFirstLoad;
}

void UToroUserSettings::AutoAdjustScalability()
{
	RunHardwareBenchmark();
	ApplyHardwareBenchmarkResults();
	SetResolutionScaleNormalized(ResPercent * 0.01f);
}

void UToroUserSettings::SetAdjustedFullscreenMode()
{
#if WITH_EDITOR
	SetFullscreenMode(EWindowMode::Windowed);
#else
	EWindowMode::Type WindowMode = EWindowMode::WindowedFullscreen;
	if (!GetBorderless())
	{
		WindowMode = (GetScreenResolution() == GetFullscreenResolution())
			? EWindowMode::Fullscreen : EWindowMode::Windowed;
	}
	SetFullscreenMode(WindowMode);
#endif
}

void UToroUserSettings::SetResolutionPercent(const uint8 InValue)
{
	if (ResPercent != InValue)
	{
		ResPercent = InValue;
		SetResolutionScaleNormalized(ResPercent * 0.01f);
	}
}

void UToroUserSettings::SetOverallQuality(const uint8 InValue)
{
	if (InValue > 0)
	{
		SetViewDistanceQuality(InValue - 1);
		SetAntiAliasingQuality(InValue - 1);
		SetShadowQuality(InValue - 1);
		SetGlobalIlluminationQuality(InValue - 1);
		SetReflectionQuality(InValue - 1);
		SetPostProcessingQuality(InValue - 1);
		SetTextureQuality(InValue - 1);
		SetVisualEffectQuality(InValue - 1);
		SetFoliageQuality(InValue - 1);
		SetShadingQuality(InValue - 1);
	}
}

uint8 UToroUserSettings::GetOverallQuality() const
{
	const int32 Target = GetViewDistanceQuality();
	if (Target == GetAntiAliasingQuality()
		&& Target == GetShadowQuality()
		&& Target == GetGlobalIlluminationQuality()
		&& Target == GetReflectionQuality()
		&& Target == GetPostProcessingQuality()
		&& Target == GetTextureQuality()
		&& Target == GetVisualEffectQuality()
		&& Target == GetFoliageQuality()
		&& Target == GetShadingQuality())
	{
		return Target + 1;
	}

	return 0;
}

void UToroUserSettings::SetAudioVolume(const ESoundClassType InType, const uint8 InVolume)
{
	if (InType == ESoundClassType::MAX) return;
	AudioVolume.FindOrAdd(InType) = FMath::Clamp(InVolume, 0, 150);
	ApplyAudioVolume();
}

void UToroUserSettings::SetSensitivityX(const float InValue)
{
	Sensitivity.X = FMath::Clamp(InValue, 0.2f, 2.0f);
}

void UToroUserSettings::SetSensitivityY(const float InValue)
{
	Sensitivity.Y = FMath::Clamp(InValue, 0.2f, 2.0f);
}

FVector2D UToroUserSettings::GetRawSensitivity() const
{
	return Sensitivity.GetAbs().ClampAxes(0.2f, 2.0f);
}

FVector2D UToroUserSettings::GetSensitivity() const
{
	FVector2D Result = GetRawSensitivity();
	return {
		GetInvertMouse().X ? -Result.X : Result.X,
		GetInvertMouse().Y ? -Result.Y : Result.Y,
	};
}

DEFINE_PROPERTY_FUNC(bool, ShowFPS, OnSettingsApply(Dynamic))
DEFINE_PROPERTY_FUNC(bool, DeveloperMode, OnSettingsApply(Developer))
DEFINE_PROPERTY_FUNC(EGameDifficulty, Difficulty, OnSettingsApply(Difficulty))

DEFINE_PROPERTY_FUNC(bool, SmoothCamera,)

DEFINE_PROPERTY_FUNC(bool, Borderless, SetAdjustedFullscreenMode();)

DEFINE_PROPERTY_FUNC(bool, FancyBloom, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC(bool, SSFogScattering, ApplySSFogScattering();)
DEFINE_PROPERTY_FUNC_CLAMPED(float, Gamma, 0.5f, 5.0f, ApplyScreenGamma();)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, Brightness, 10, 200, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, MotionBlur, 0, 3, OnSettingsApply(Dynamic);)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, LumenGI, 0, 3, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, LumenReflection, 0, 3, OnSettingsApply(Dynamic);)
DEFINE_PROPERTY_FUNC(bool, HitLighting, OnSettingsApply(Dynamic);)

DEFINE_PROPERTY_FUNC(EImageFidelityMode, ImageFidelity, ApplyImageFidelity();)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, FSRQuality, 0, 4, ApplyFSR();)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, FSRSharpness, 0, 100, ApplyFSR();)
DEFINE_PROPERTY_FUNC(bool, FSRFrameGen, ApplyFSR();)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, XeSSQuality, 0, 6, ApplyXeSS();)
DEFINE_PROPERTY_FUNC(bool, XeSSFrameGen, ApplyXeSS();)
DEFINE_PROPERTY_FUNC(bool, XeSSLowLatency, ApplyXeSS();)

DEFINE_PROPERTY_FUNC_CLAMPED(uint8, DLSSQuality, 0, 6, ApplyDLSS();)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, DLSSFrameGen, 0, 4, ApplyDLSS();)
DEFINE_PROPERTY_FUNC_CLAMPED(uint8, DLSSReflex, 0, 2, ApplyDLSS();)
DEFINE_PROPERTY_FUNC(bool, DLSSRayReconstruct, ApplyDLSS();)

void UToroUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
#if WITH_EDITOR
	if (FApp::IsGame())
#endif
	{
		ApplyScreenGamma();
		ApplySSFogScattering();
		ApplyAudioVolume();
		ApplyImageFidelity();
	}

	OnSettingsApply(Manual)
	Super::ApplySettings(bCheckForCommandLineOverrides);
}

void UToroUserSettings::ApplyScreenGamma() const
{
	if (GEngine) GEngine->DisplayGamma = GetGamma();
}

void UToroUserSettings::ApplySSFogScattering() const
{
	UToroConsoleLibrary::SetCVarBool(TEXT("r.SSFS"), GetSSFogScattering());
}

void UToroUserSettings::ApplyAudioVolume() const
{
#if WITH_EDITOR
	if (!FApp::IsGame()) return;
#endif
	const UToroSettings* Settings = UToroSettings::Get();
	USoundMix* SoundMix = Settings->MainSoundMix.LoadSynchronous();
	if (!SoundMix) return;

	for (const ESoundClassType Type : TEnumRange<ESoundClassType>())
	{
		USoundClass* SoundClass = Settings->SoundClasses[static_cast<uint8>(Type)].LoadSynchronous();
		if (!SoundClass) continue;

		UGameplayStatics::SetSoundMixClassOverride(this, SoundMix, SoundClass,
			GetAudioVolume(Type), 1.0f, 0.5f, true);
	}
}

void UToroUserSettings::ApplyImageFidelity()
{
	ImageFidelity::SetAntiAliasing(GetImageFidelity());

	ApplyFSR();
	ApplyXeSS();
	ApplyDLSS();

	if (ImageFidelity::IsUsingAnyFrameGen())
	{
		SetVSyncEnabled(false);
	}

	OnSettingsApply(UI)
}

void UToroUserSettings::ApplyFSR() const
{
	const bool bFSR = GetImageFidelity() == EImageFidelityMode::FSR;

	ImageFidelity::FSR::SetEnabled(bFSR);
	ImageFidelity::FSR::SetQuality(GetFSRQuality());
	ImageFidelity::FSR::SetSharpness(GetFSRSharpness());
	ImageFidelity::FSR::SetFrameGenEnabled(bFSR && GetFSRFrameGen());
}

void UToroUserSettings::ApplyXeSS() const
{
	const bool bXeSS = ImageFidelity::XeSS::SR::IsSupported() && GetImageFidelity() == EImageFidelityMode::XeSS;

	ImageFidelity::XeSS::SR::SetMode(bXeSS, GetXeSSQuality());
	ImageFidelity::XeSS::FG::SetEnabled(bXeSS && GetXeSSFrameGen());
	ImageFidelity::XeSS::LL::SetEnabled(bXeSS && GetXeSSLowLatency());
}

void UToroUserSettings::ApplyDLSS()
{
	const bool bDLSS = GetImageFidelity() == EImageFidelityMode::DLSS;
	if (!ImageFidelity::DLSS::SR::IsSupportedModeIdx(GetDLSSQuality()))
	{
		DLSSQuality = 4;
	}
	if (!ImageFidelity::DLSS::FG::IsSupportedModeIdx(GetDLSSFrameGen()))
	{
		DLSSFrameGen = 0;
	}
	if (GetDLSSFrameGen() > 0 && GetDLSSReflex() == 0)
	{
		DLSSReflex = 1;
	}
	
	ImageFidelity::DLSS::SR::SetMode(bDLSS, GetDLSSQuality(), GetScreenResolution());
	ImageFidelity::DLSS::FG::SetMode(bDLSS ? GetDLSSFrameGen() : 0);
	ImageFidelity::DLSS::Reflex::SetMode(bDLSS ? GetDLSSReflex() : 0);
	ImageFidelity::DLSS::RR::SetMode(bDLSS && GetDLSSRayReconstruct());
}

void UToroUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	ImageFidelity::FSR::Initialize();
	UpdateResolutions();

	ShowFPS = false;
	DeveloperMode = false;
	Difficulty = EGameDifficulty::Unset;
	
	SmoothCamera = true;
	Sensitivity = FVector2D::UnitVector;
	InvertMouse = FMouseInversion::Disabled;
	
	Borderless = true;
	ResPercent = 100;
	bUseVSync = false;
	FrameRateLimit = 60.0f;
	
	Gamma = 2.2;
	Brightness = 100;
	FancyBloom = true;
	SSFogScattering = true;
	MotionBlur = 1;
	
	LumenGI = 0;
	LumenReflection = 2;
	HitLighting = false;
	
	ImageFidelity = EImageFidelityMode::TSR;
	
	FSRQuality = 2;
	FSRSharpness = 0.0f;
	FSRFrameGen = false;
	
	XeSSQuality = 2;
	XeSSFrameGen = false;
	XeSSLowLatency = false;

	DLSSQuality = 4;
	DLSSFrameGen = 0;
	DLSSReflex = 0;
	DLSSRayReconstruct = false;

	SetOverallScalabilityLevel(3);

	AudioVolume = {
		{ESoundClassType::Master, 100},
		{ESoundClassType::Music, 100},
		{ESoundClassType::SFX, 100},
		{ESoundClassType::Voice, 100}
	};
}

UWorld* UToroUserSettings::GetWorld() const
{
	UWorld* World = Super::GetWorld();
	if (!World) World = GameInstance ? GameInstance->GetWorld() : nullptr;
	if (!World) World = GEngine->GetCurrentPlayWorld();
	return World;
}

#undef OnSettingsApply
#undef DEFINE_PROPERTY_FUNC
#undef DEFINE_PROPERTY_FUNC_CLAMPED