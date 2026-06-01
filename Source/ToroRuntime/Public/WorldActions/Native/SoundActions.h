// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Sound/AmbientSound.h"
#include "DataTypes/MiscEnums.h"
#include "WorldActions/WorldActionBase.h"
#include "SoundActions.generated.h"

/** 
 * Plays a non-spatialized 2D sound effect. 
 * Ideal for UI sounds, background music, or global voice-overs that do not have a location in 3D space.
 */
UCLASS(NotBlueprintable, DisplayName = "[Sound] Simple (2D)")
class TORORUNTIME_API UWorldAction_Sound2D : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Sound2D()
		: Volume(1.0f), Pitch(1.0f), StartTime(0.0f)
	{}

private:

	/** The sound asset to play. Handled as a Soft Pointer to avoid loading the asset until the action runs. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<USoundBase> Sound;

	/** Multiplier for the sound volume. */
	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;

	/** Multiplier for the sound pitch/speed. */
	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Pitch;

	/** The timestamp (in seconds) within the audio file to begin playback. */
	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float StartTime;

	virtual void OnExecute_Implementation() override;
};

/** 
 * Controls an existing spatialized AAmbientSound actor in the level.
 * Supports Play, Stop, and Fade operations.
 */
UCLASS(NotBlueprintable, DisplayName = "[Sound] World (3D)")
class TORORUNTIME_API UWorldAction_Sound3D : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Sound3D()
		: Action(EAudioComponentAction::Play) 
	{}

private:

	/** The target Ambient Sound actor in the current level. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<AAmbientSound> SoundActor;

	/** The specific audio command to send to the target actor. */
	UPROPERTY(EditAnywhere, Category = Action)
		EAudioComponentAction Action;

	/** Start time offset for the 'Play' action. */
	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Action == EAudioComponentAction::Play", EditConditionHides, ClampMin = 0.0f, UIMin = 0.0f))
		float StartTime;

	/** Duration of the volume ramp-up (Play) or ramp-down (Stop). */
	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Action == EAudioComponentAction::Play || Action == EAudioComponentAction::Stop", EditConditionHides, ClampMin = 0.0f, UIMin = 0.0f))
		float FadeTime;

	virtual void OnExecute_Implementation() override;
};