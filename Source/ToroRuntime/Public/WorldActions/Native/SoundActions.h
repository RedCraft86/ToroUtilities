// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Sound/AmbientSound.h"
#include "DataTypes/MiscEnums.h"
#include "WorldActions/WorldActionBase.h"
#include "SoundActions.generated.h"

UCLASS(NotBlueprintable, DisplayName = "[Sound] Simple (2D)")
class TORORUNTIME_API UWorldAction_Sound2D : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Sound2D()
		: Volume(1.0f), Pitch(1.0f), StartTime(0.0f)
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Pitch;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float StartTime;

	virtual void OnExecute_Implementation() override;
};

UCLASS(NotBlueprintable, DisplayName = "[Sound] World (3D)")
class TORORUNTIME_API UWorldAction_Sound3D : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_Sound3D()
		: Action(EAudioComponentAction::Play) 
	{}

private:

	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<AAmbientSound> SoundActor;

	UPROPERTY(EditAnywhere, Category = Action)
		EAudioComponentAction Action;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Action == EAudioComponentAction::Play", EditConditionHides, ClampMin = 0.0f, UIMin = 0.0f))
		float StartTime;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "Action == EAudioComponentAction::Play || Action == EAudioComponentAction::Stop", EditConditionHides, ClampMin = 0.0f, UIMin = 0.0f))
		float FadeTime;

	virtual void OnExecute_Implementation() override;
};