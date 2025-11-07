// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"
#include "Gameplay/WorldActions/WorldActionBase.h"
#include "SoundActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Sounds", meta = (Hidden))
struct TORORUNTIME_API FWATask_Sounds : public FWorldActionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "2D Sound")
struct TORORUNTIME_API FWTSounds_Simple final : public FWATask_Sounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Volume;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f))
		float Pitch;

	UPROPERTY(EditAnywhere, Category = Action)
		TObjectPtr<USoundBase> Sound;
	
	FWTSounds_Simple(): Volume(1.0f), Pitch(1.0f) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "3D Sound")
struct TORORUNTIME_API FWTSounds_World final : public FWATask_Sounds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		bool bStopSound;
	
	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.0f, UIMin = 0.0f, EditCondition = "!bStopSound", EditConditionHides))
		float StartTime;
	
	UPROPERTY(EditAnywhere, Category = Action)
		bool bFade;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.1f, UIMin = 0.1f, EditCondition = "bFade", EditConditionHides))
		float FadeTime;
	
	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "bFade", EditConditionHides))
		EAudioFaderCurve FadeCurve;
	
	UPROPERTY(EditAnywhere, Category = Action)
	// ReSharper disable once UnrealHeaderToolError
	TSet<TSoftObjectPtr<AAmbientSound>> Targets;
	
	FWTSounds_World()
		: bStopSound(false), StartTime(0.0f), bFade(false)
		, FadeTime(1.0f), FadeCurve(EAudioFaderCurve::Linear)
	{}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};