// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroWorldSettings.h"
#include "PostProcess/GlobalPostProcess.h"
#include "LightProbes/LightProbeManager.h"
#include "Sound/AudioSettings.h"
#include "Sound/SoundMix.h"
#include "AudioDevice.h"

AToroWorldSettings::AToroWorldSettings()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.TickGroup = TG_DuringPhysics;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	PostProcess = CreateDefaultSubobject<UGlobalPostProcess>(TEXT("GlobalPostProcess"));
	PostProcess->SetupAttachment(SceneRoot);

	LightProbes = CreateDefaultSubobject<ULightProbeManager>(TEXT("LightProbeManager"));

	bEnableAutoLODGeneration = false; // Include Actor in HLOD option

	SetCanBeDamaged(false);
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
}
#endif