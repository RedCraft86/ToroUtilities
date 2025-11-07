// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "LevelSequenceActor.h"
#include "Gameplay/Cutscenes/ToroCutsceneActor.h"
#include "Gameplay/WorldActions/WorldActionBase.h"
#include "SequenceActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Sequence", meta = (Hidden))
struct TORORUNTIME_API FWATask_Sequence : public FWorldActionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Basic Sequence")
struct TORORUNTIME_API FWTSequence_Basic final : public FWATask_Sequence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		bool bStopPlaying;

	UPROPERTY(EditAnywhere, Category = Action, meta = (EditCondition = "!bStopPlaying", EditConditionHides))
		float PlayRate;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<ALevelSequenceActor>> Targets;

	FWTSequence_Basic(): bStopPlaying(false), PlayRate(1.0f) {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintType, DisplayName = "Cutscene")
struct TORORUNTIME_API FWTSequence_Cutscene final : public FWATask_Sequence
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 0.0f, UIMin = 0.0f))
		float FadeTime;

	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<AToroCutsceneActor> Target;

	FWTSequence_Cutscene(): FadeTime(0.5f) {}

protected:

	virtual void RunAction() override;
};