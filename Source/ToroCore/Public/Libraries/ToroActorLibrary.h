// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "LevelSequenceActor.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroActorLibrary.generated.h"

UENUM(BlueprintType)
enum class ESequenceStopType : uint8
{
	/** Use the default engine behavior for stopping a sequence. */
	Default,

	/** Halts the sequence exactly at the current playback frame. */
	AtCurrentTime,

	/** Instantly jumps the sequence to the final frame and then stops. 
		Useful for "Skipping" cinematics while ensuring world state is updated. */
	SkipToEnd
};

/**
 * Provides utility functions for common Actor, Level Sequence, and Audio operations.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroActorLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Calculates the bounding box vertices of an actor in world space.
	 * @param OutVerts       The set of 8 world-space vertices forming the box.
	 * @param Origin         The center point of the bounding box.
	 * @param Extent         The half-size dimensions of the box.
	 * @param Target         The actor to analyze.
	 * @param Scale          Optional scale multiplier for the resulting box.
	 * @param bOnlyColliding If true, only components with collision enabled contribute to the box.
	 * @param bChildActors   If true, includes the bounding boxes of all child actors in the calculation.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor, meta = (DefaultToSelf = "Target"))
		static void GetBoundingBoxVertices(TSet<FVector>& OutVerts, FVector& Origin, FVector& Extent, const AActor* Target, 
			const FVector Scale = FVector(1, 1, 1), const bool bOnlyColliding = false, const bool bChildActors = true);

	/** Adds a unique Tag to the actor's Tags array. */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Add Tag", meta = (DefaultToSelf = "Target"))
		static void AddActorTag(AActor* Target, const FName InTag);

	/** Removes a specific Tag from the actor's Tags array. */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Remove Tag", meta = (DefaultToSelf = "Target"))
		static void RemoveActorTag(AActor* Target, const FName InTag);

	/** Initiates playback of a Level Sequence at a specified rate. */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Play Sequence", meta = (DefaultToSelf = "Target"))
		static void PlayLevelSequence(const ALevelSequenceActor* Target, const float PlayRate = 1.0f);

	/** Plays the Level Sequence in reverse from its current position. */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Reverse Sequence", meta = (DefaultToSelf = "Target"))
		static void ReverseLevelSequence(const ALevelSequenceActor* Target, const float PlayRate = 1.0f);

	/** 
	 * Stops the sequence based on the chosen StopType.
	 * Use 'SkipToEnd' to ensure any 'Keep State' tracks are fully applied before the cinematic ends.
	 */
	UFUNCTION(BlueprintCallable, Category = LevelSequence, DisplayName = "Stop Sequence", meta = (DefaultToSelf = "Target"))
		static void StopLevelSequence(const ALevelSequenceActor* Target, const ESequenceStopType StopType = ESequenceStopType::Default);

	/** Starts playback of an Ambient Sound actor. */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Play Sound", meta = (DefaultToSelf = "Target"))
		static void PlayAmbientSound(const AAmbientSound* Target, float StartTime = 0.0f);

	/** Immediately stops an Ambient Sound actor. */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Stop Sound", meta = (DefaultToSelf = "Target"))
		static void StopAmbientSound(const AAmbientSound* Target);

	/** Gradually increases the volume of an Ambient Sound. */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade In Sound", meta = (DefaultToSelf = "Target"))
		static void FadeInAmbientSound(const AAmbientSound* Target, float Duration = 1.0f, float TargetLevel = 1.0f,
			float StartTime = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);

	/** Gradually decreases the volume of an Ambient Sound to a target level (usually 0). */
	UFUNCTION(BlueprintCallable, Category = Audio, DisplayName = "Fade Out Sound", meta = (DefaultToSelf = "Target"))
		static void FadeOutAmbientSound(const AAmbientSound* Target, float Duration = 1.0f,
			float TargetLevel = 0.0f, const EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear);
};
