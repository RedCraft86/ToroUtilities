// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

// ReSharper disable UnrealHeaderToolError
#pragma once

#include "Components/AudioComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroSoundLibrary.generated.h"

class AAmbientSound;

/**
 * A utility library providing functions for game sounds.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroSoundLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Starts playback of an Ambient Sound actor.
	 * @param Target Ambient sound to play.
	 * @param StartTime Time of the sound to start playback at.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Play Sound", meta = (DefaultToSelf = Target))
		static void PlayAmbientSound(const AAmbientSound* Target, const float StartTime = 0.0f);

	/**
	 * Immediately stops an Ambient Sound actor.
	 * @param Target Ambient sound to stop.
	 * @param Delay (Optional) Time to wait before stopping.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Stop Sound", meta = (DefaultToSelf = Target))
		static void StopAmbientSound(const AAmbientSound* Target, const float Delay = 0.0f);

	/**
	 * Gradually increases the volume of an Ambient Sound.
	 * @param Target Ambient sound to fade in.
	 * @param Duration How long it should take to complete the fade in.
	 * @param TargetLevel Volume adjustment to fade in to.
	 * @param StartTime Time of the sound to start playback at.
	 * @param FadeCurve Alpha curve to use when processing the fade.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade In Sound", meta = (DefaultToSelf = Target))
		static void FadeInAmbientSound(const AAmbientSound* Target, const float Duration = 1.0f, const float TargetLevel = 1.0f,
			const float StartTime = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);

	/**
	 * Gradually decreases the volume of an Ambient Sound to a target level (usually 0).
	 * @param Target Ambient sound to fade out.
	 * @param Duration How long it should take to complete the fade out.
	 * @param TargetLevel Volume adjustment to fade out to.
	 * @param FadeCurve Alpha curve to use when processing the fade.
	 */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade Out Sound", meta = (DefaultToSelf = Target))
		static void FadeOutAmbientSound(const AAmbientSound* Target, const float Duration = 1.0f,
			const float TargetLevel = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);
};
