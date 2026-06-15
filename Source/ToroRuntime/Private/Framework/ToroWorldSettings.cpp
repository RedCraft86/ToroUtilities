// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "Framework/ToroWorldSettings.h"
#include "WorldMusic/WorldMusicManager.h"
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

	WorldMusic = CreateDefaultSubobject<UWorldMusicManager>(TEXT("WorldMusicManager"));
#if WITH_EDITORONLY_DATA
	WorldMusic->bVisualizeComponent = false;
#endif
	WorldMusic->SetupAttachment(SceneRoot);

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