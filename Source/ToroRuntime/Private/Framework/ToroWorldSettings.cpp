// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroWorldSettings.h"
#include "LightProbes/LightProbeManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Libraries/ToroWorldLibrary.h"
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

	LightProbes = CreateDefaultSubobject<ULightProbeManager>(TEXT("LightProbes"));

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
	if (!InMaterial)
	{
		return nullptr;
	}

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

	return nullptr;
}

UMaterialInterface* AToroWorldSettings::FindOrAddBlendable(UMaterialInterface* InMaterial, const bool bDynamic)
{
	if (!InMaterial)
	{
		return nullptr;
	}

	UMaterialInterface* Blendable = FindBlendable(InMaterial);
	if (Blendable)
	{
		// Reset weight to 1 just in case
		PostProcessing.AddBlendable(Blendable, 1.0f);
		return Blendable;
	}

	Blendable = bDynamic ? UMaterialInstanceDynamic::Create(InMaterial, this) : InMaterial;
	PostProcessing.AddBlendable(Blendable, 1.0f);
	PostProcessTickInterval.ForceReady();

	return Blendable;
}

void AToroWorldSettings::RemoveBlendable(UMaterialInterface* InMaterial)
{
	if (UMaterialInterface* Blendable = FindBlendable(InMaterial))
	{
		PostProcessing.RemoveBlendable(Blendable);
		PostProcessTickInterval.ForceReady();
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
		const ELumenUsageMode LumenMode = UserSettings->GetLumenMode();

		PostProcessing.bOverride_ReflectionMethod = true;
		PostProcessing.ReflectionMethod = LumenReflectionAllowed(LumenMode) 
			? EReflectionMethod::Lumen : EReflectionMethod::ScreenSpace;

		PostProcessing.bOverride_DynamicGlobalIlluminationMethod = true;
		PostProcessing.DynamicGlobalIlluminationMethod = LumenLightingAllowed(LumenMode) 
			? EDynamicGlobalIlluminationMethod::Lumen : EDynamicGlobalIlluminationMethod::None;

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

	if (bAdvancedLumenCheck)
	{
		float HighestPriority = PostProcess->Priority;
		const FVector ViewLocation = UToroWorldLibrary::GetMainCameraTransform(this).GetLocation();
		for (IInterface_PostProcessVolume& PPVolume : GetWorld()->GetPostProcessVolumeIterator())
		{
			const FPostProcessVolumeProperties Prop = PPVolume.GetProperties();
			if (Prop.Settings && Prop.Priority > HighestPriority 
				&& Prop.Settings->bOverride_DynamicGlobalIlluminationMethod 
				&& (Prop.bIsUnbound || PPVolume.EncompassesPoint(ViewLocation, 0.0f, nullptr)))
			{
				HighestPriority = Prop.Priority;
				bUsesLumenGI = Prop.Settings->DynamicGlobalIlluminationMethod 
								== EDynamicGlobalIlluminationMethod::Lumen;
			}
		}
	}
}

void AToroWorldSettings::BeginPlay()
{
	Super::BeginPlay();
	PostProcessTickInterval.ForceReady();

	// Fixes motion blur issues when paused
	GetWorld()->bIsCameraMoveableWhenPaused = true;
}

void AToroWorldSettings::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (PostProcessTickInterval.TickCooldown(DeltaSeconds))
	{
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