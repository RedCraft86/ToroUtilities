// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroWorldSettings.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Sound/AudioSettings.h"
#include "Sound/SoundMix.h"
#include "AudioDevice.h"
#include "RenderUtils.h"
#include "SceneInterface.h"

AToroWorldSettings::AToroWorldSettings()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PostProcess = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcess"));
	PostProcess->SetupAttachment(SceneRoot);
	PostProcess->BlendWeight = 1.0f;
	PostProcess->Priority = -1.0f;
	PostProcess->bUnbound = true;
	PostProcess->bEnabled = true;

	PostProcessTickInterval = 0.1f;
	PostProcessTick = PostProcessTickInterval;
	bAdvancedLumenCheck = true;

	PostProcessing.bOverride_AutoExposureMethod = true;
	PostProcessing.AutoExposureMethod = AEM_Manual;

	PostProcessing.bOverride_AutoExposureBias = true;
	PostProcessing.AutoExposureBias = 11.0f;

	PostProcessing.bOverride_BloomMethod = true;
	PostProcessing.BloomMethod = EBloomMethod::BM_FFT;

	PostProcess->Settings = PostProcessing;

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
}

UMaterialInterface* AToroWorldSettings::FindBlendable(const UMaterialInterface* InMaterial) const
{
	if (InMaterial)
	{
		for (const FWeightedBlendable& Blendable : PostProcessing.WeightedBlendables.Array)
		{
			if (UMaterialInterface* Material = Cast<UMaterialInterface>(Blendable.Object))
			{
				if (Material == InMaterial)
				{
					return Material;
				}

				const UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(Material);
				if (MID && MID->Parent == InMaterial)
				{
					return Material;
				}
			}
		}
	}

	return nullptr;
}

UMaterialInterface* AToroWorldSettings::FindOrAddBlendable(UMaterialInterface* InMaterial, const bool bDynamic)
{
	if (InMaterial)
	{
		UMaterialInterface* Blendable = FindBlendable(InMaterial);
		if (Blendable)
		{
			// Set weight to 1 just in case
			PostProcessing.AddBlendable(Blendable, 1.0f);
			return Blendable;
		}

		Blendable = bDynamic ? UMaterialInstanceDynamic::Create(InMaterial, this) : InMaterial;
		PostProcessing.AddBlendable(Blendable, 1.0f);
		PostProcessTick = PostProcessTickInterval;
		UpdatePostProcess();

		return Blendable;
	}

	return nullptr;
}

void AToroWorldSettings::RemoveBlendable(UMaterialInterface* InMaterial)
{
	if (UMaterialInterface* Blendable = FindBlendable(InMaterial))
	{
		PostProcessing.RemoveBlendable(Blendable);
		PostProcessTick = PostProcessTickInterval;
		UpdatePostProcess();
	}
}

void AToroWorldSettings::SetSoundVolume(USoundClass* InSoundClass, float InVolume) const
{
	if (DefaultBaseSoundMix && InSoundClass && GEngine && GEngine->UseSound() && GetWorld()->bAllowAudioPlayback)
	{
		if (FAudioDeviceHandle AudioDevice = GetWorld()->GetAudioDevice())
		{
			AudioDevice->SetSoundMixClassOverride(DefaultBaseSoundMix, InSoundClass, InVolume, 1.0f, 0.0f, true);
		}
	}
}

void AToroWorldSettings::UpdatePostProcess()
{
#if WITH_EDITOR
	if (FApp::IsGame())
#endif
	{
		// TODO ELumenUsageMode LumenMode = UserSettings->GetLumenMode();
		//
		// Settings.bOverride_ReflectionMethod = true;
		// Settings.ReflectionMethod = LumenReflectionAllowed(LumenMode) 
		// 	? EReflectionMethod::Lumen : EReflectionMethod::ScreenSpace;
		//
		// Settings.bOverride_DynamicGlobalIlluminationMethod = true;
		// Settings.DynamicGlobalIlluminationMethod = LumenLightingAllowed(LumenMode) 
		// 	? EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::ScreenSpace;

		if (UserSettings->GetGlobalIlluminationQuality() >= 4)
		{
			PostProcessing.LumenRayLightingMode = ELumenRayLightingModeOverride::HitLighting;
		}
		else
		{
			PostProcessing.LumenRayLightingMode = UserSettings->GetReflectionQuality() >= 3
				? ELumenRayLightingModeOverride::HitLightingForReflections
				: ELumenRayLightingModeOverride::SurfaceCache;
		}
	}

	PostProcess->Settings = PostProcessing;
}

void AToroWorldSettings::UpdateLumenGIUsage()
{
	const IConsoleManager& CM = IConsoleManager::Get();
	static const auto GIAllowed = CM.FindTConsoleVariableDataInt(TEXT("r.Lumen.DiffuseIndirect.Allow"));
	if (!GIAllowed->GetValueOnGameThread() || !DoesPlatformSupportLumenGI(PostProcess->GetScene()->GetShaderPlatform()))
	{
		bUsesLumenGI = false;
		return;
	}

	if (!PostProcess->Settings.bOverride_DynamicGlobalIlluminationMethod)
	{
		static const auto GIMethod = CM.FindTConsoleVariableDataInt(TEXT("r.DynamicGlobalIlluminationMethod"));
		bUsesLumenGI = GIMethod->GetValueOnGameThread() == static_cast<int32>(EDynamicGlobalIlluminationMethod::Lumen);
	}
	else
	{
		bUsesLumenGI = PostProcess->Settings.DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen;
	}

	if (!bAdvancedLumenCheck)
	{
		return;
	}

	FVector ViewLocation = FVector::ZeroVector;
	if (const APlayerCameraManager* CamManager = CameraManager.Get())
	{
		ViewLocation = CamManager->GetCameraLocation();
	}

	float HighestPriority = PostProcess->Priority;
	for (IInterface_PostProcessVolume& PPVolume : GetWorld()->GetPostProcessVolumeIterator())
	{
		const FPostProcessVolumeProperties Prop = PPVolume.GetProperties();
		if (Prop.Settings->bOverride_DynamicGlobalIlluminationMethod && Prop.Priority > HighestPriority
			&& (Prop.bIsUnbound || PPVolume.EncompassesPoint(ViewLocation, 0.0f, nullptr)))
		{
			HighestPriority = Prop.Priority;
			bUsesLumenGI = Prop.Settings->DynamicGlobalIlluminationMethod == EDynamicGlobalIlluminationMethod::Lumen;
		}
	}
}

void AToroWorldSettings::BeginPlay()
{
	Super::BeginPlay();

	// Fixes motion blur issues when paused
	GetWorld()->bIsCameraMoveableWhenPaused = true;

	UpdatePostProcess();
}

void AToroWorldSettings::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if ((PostProcessTick -= DeltaSeconds) < 0.0f)
	{
		PostProcessTick = PostProcessTickInterval;
		UpdatePostProcess();
		UpdateLumenGIUsage();
	}
}

#if WITH_EDITOR
void AToroWorldSettings::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!DefaultBaseSoundMix)
	{
		if (const UAudioSettings* Settings = GetDefault<UAudioSettings>())
		{
			DefaultBaseSoundMix = Cast<USoundMix>(Settings->DefaultBaseSoundMix.TryLoad());
		}
	}

	UpdatePostProcess();
}
#endif