// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "SceneUtils.h"
#include "Misc/EnumRange.h"
#include "UserSettingTypes.generated.h"

/**
 * Defines how Lumen (Global Illumination and Reflections) is utilized in the game.
 */
UENUM(BlueprintInternalUseOnly)
enum class ELumenUsageMode : uint8
{
	/** Lumen is completely disabled. Fallback to SSR and static probe based fake GI. */
	Disabled,

	/** Only Lumen Reflections are active; Global Illumination is faked with static probes. */
	ReflectionOnly,

	/** Only Lumen Global Illumination is active; Reflections use SSR. */
	LightingOnly,

	/** Both Lumen Global Illumination and Lumen Reflections are fully enabled. */
	Enabled
};
inline bool LumenReflectionAllowed(const ELumenUsageMode Mode)
{
	return Mode == ELumenUsageMode::Enabled || Mode == ELumenUsageMode::ReflectionOnly;
}
inline bool LumenLightingAllowed(const ELumenUsageMode Mode)
{
	return Mode == ELumenUsageMode::Enabled || Mode == ELumenUsageMode::LightingOnly;
}

/**
 * Supported Anti-Aliasing and Upscaling methods to define visual fidelity.
 */
UENUM(BlueprintInternalUseOnly)
enum class EImageFidelityMode : uint8
{
	/** No Anti-Aliasing or Upscaling (Raw output). */
	None,

	/** Fast Approximate Anti-Aliasing. Low cost, slight blurring. */
	FXAA,

	/** Subpixel Morphological Anti-Aliasing. Better edges than FXAA. */
	SMAA,

	/** Conservative Morphological Anti-Aliasing 2. Better than FXAA but may or may not be better than TAA. */
	CMAA2,

	/** Temporal Anti-Aliasing. Standard UE4/UE5 temporal solution. */
	TAA,

	/** Temporal Super-Resolution. Epic's high-quality AA + Upscaler. */
	TSR,

	/** AMD FidelityFX Super Resolution 4. Hardware-agnostic upscaler. */
	FSR,

	/** Intel XeSS Super Resolution. AI-based upscaler. */
	XeSS,

	/** NVIDIA Deep Learning Super Sampling. AI-based upscaler (Requires RTX). */
	DLSS,

	/** Helper value for counting and range-based loops. */
	MAX UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EImageFidelityMode, EImageFidelityMode::MAX);

/**
 * Represents a specific audio channel (SoundClass) and its current volume level.
 * Used for persisting user audio preferences in ToroGameUserSettings.
 */
USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FAudioChannelVolume
{
	GENERATED_BODY()

	/** The Unique Name (FName) of the audio channel/SoundClass. */
	UPROPERTY() FName Channel;

	/** The volume level ranging from 0 to 100. */
	UPROPERTY() uint8 Volume;

	FAudioChannelVolume(const FName InChannel = NAME_None)
		: Channel(InChannel), Volume(100)
	{}

	/**
	 * Checks if the channel is not MAX.
	 */
	FORCEINLINE bool IsValidChannel() const { return !Channel.IsNone(); }

	FORCEINLINE bool operator==(const FAudioChannelVolume& Other) const
	{
		return Channel == Other.Channel;
	}

	FORCEINLINE bool operator!=(const FAudioChannelVolume& Other) const
	{
		return Channel != Other.Channel;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FAudioChannelVolume& InSetting)
	{
		return GetTypeHash(InSetting.Channel);
	}
};