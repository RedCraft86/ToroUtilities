// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroSequencerLibrary.generated.h"

class ALevelSequenceActor;

/**
 * The method on which how the level sequence will be stopped.
 */
UENUM(BlueprintType)
enum class EToroSequenceStopType : uint8
{
	// Stop the sequence using Stop() with no additional behavior.
	Default,

	// Jumps the sequence to the first frame after stopping.
	Reset,

	// Stops the sequence exactly at the current playback frame.
	CurrentTime,

	// Scrubs the sequence to the final frame after stopping.
	SkipToEnd
};

/**
 * A utility library providing functions for the game sequencer.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroSequencerLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Initiates playback of a Level Sequence at a specified rate.
	 * @param Target Level sequence to play.
	 * @param PlayRate Speed at which playback occurs.
	 */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Play Sequence", meta = (DefaultToSelf = Target))
		static void PlayLevelSequence(const ALevelSequenceActor* Target, const float PlayRate = 1.0f);

	/**
	 * Plays the Level Sequence in reverse from its current position.
	 * @param Target Level sequence to reverse.
	 * @param PlayRate Speed at which playback occurs.
	 */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Reverse Sequence", meta = (DefaultToSelf = Target))
		static void ReverseLevelSequence(const ALevelSequenceActor* Target, const float PlayRate = 1.0f);

	/**
	 * Stops the sequence based on the chosen StopType.
	 * @param Target Level sequence to stop.
	 * @param StopType The way in which it should be stopped
	 */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Stop Sequence", meta = (DefaultToSelf = Target))
		static void StopLevelSequence(const ALevelSequenceActor* Target, const EToroSequenceStopType StopType);
};
