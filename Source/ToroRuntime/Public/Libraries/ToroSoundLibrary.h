// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

// ReSharper disable UnrealHeaderToolError
#pragma once

#include "UE5Coro.h"
#include "Components/AudioComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroSoundLibrary.generated.h"

class AAmbientSound;

/**
 * Controls playback and fades on ambient sound actors.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroSoundLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Plays an ambient sound from the specified offset.
	 * @param Target Ambient sound actor to play.
	 * @param StartTime Playback start time in seconds.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Play Sound", meta = (DefaultToSelf = Target))
		static void PlayAmbientSound(const AAmbientSound* Target, const float StartTime = 0.0f);

	/**
	 * Stops an ambient sound immediately or after a delay.
	 * @param Target Ambient sound actor to stop.
	 * @param Delay Seconds to wait before stopping.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Stop Sound", meta = (DefaultToSelf = Target))
		static void StopAmbientSound(const AAmbientSound* Target, const float Delay = 0.0f);

	/**
	 * Fades in an ambient sound and completes after the fade duration.
	 * @param Target Ambient sound actor to fade in.
	 * @param Duration Fade duration in seconds.
	 * @param TargetLevel Final volume multiplier.
	 * @param StartTime Playback start time in seconds.
	 * @param FadeCurve Fade interpolation curve.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade In Sound", meta = (Latent, LatentInfo = LatentInfo, DefaultToSelf = Target))
		static FVoidCoroutine FadeInAmbientSound(FLatentActionInfo LatentInfo, const AAmbientSound* Target, const float Duration = 1.0f,
			const float TargetLevel = 1.0f, const float StartTime = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);

	/**
	 * Fades out an ambient sound and completes after the fade duration.
	 * @param Target Ambient sound actor to fade out.
	 * @param Duration Fade duration in seconds.
	 * @param TargetLevel Final volume multiplier.
	 * @param FadeCurve Fade interpolation curve.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade Out Sound", meta = (Latent, LatentInfo = LatentInfo, DefaultToSelf = Target))
		static FVoidCoroutine FadeOutAmbientSound(FLatentActionInfo LatentInfo, const AAmbientSound* Target, const float Duration = 1.0f,
			const float TargetLevel = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);
};
