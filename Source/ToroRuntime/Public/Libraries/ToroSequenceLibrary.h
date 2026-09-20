// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroSequenceLibrary.generated.h"

class ALevelSequenceActor;

/**
 * Specifies how a level sequence stops.
 */
UENUM(BlueprintType)
enum class EToroSequenceStopType : uint8
{
	Default,
	Reset,
	CurrentTime,
	SkipToEnd
};

/**
 * Controls playback of a level sequence actor.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroSequenceLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Plays a sequence using the specified play rate.
	 * @param Target Sequence actor to play.
	 * @param PlayRate Playback speed multiplier.
	 */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Play Sequence", meta = (DefaultToSelf = Target))
		static void PlayLevelSequence(const ALevelSequenceActor* Target, const float PlayRate = 1.0f);

	/**
	 * Plays a sequence in reverse using the specified play rate.
	 * @param Target Sequence actor to reverse.
	 * @param PlayRate Playback speed multiplier.
	 */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Reverse Sequence", meta = (DefaultToSelf = Target))
		static void ReverseLevelSequence(const ALevelSequenceActor* Target, const float PlayRate = 1.0f);

	/**
	 * Stops a sequence using the requested stop behavior.
	 * @param Target Sequence actor to stop.
	 * @param StopType Whether to stop normally, reset, retain the current time, or skip to the end.
	 */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Stop Sequence", meta = (DefaultToSelf = Target))
		static void StopLevelSequence(const ALevelSequenceActor* Target, const EToroSequenceStopType StopType);
};
