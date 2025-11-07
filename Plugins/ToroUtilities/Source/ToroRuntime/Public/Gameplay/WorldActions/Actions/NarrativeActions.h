// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Quest.h"
#include "Dialogue.h"
#include "NarrativeDataTask.h"
#include "Gameplay/WorldActions/WorldActionBase.h"
#include "NarrativeActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Narrative", meta = (Hidden))
struct TORORUNTIME_API FWATask_Narrative : public FWorldActionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Start Dialogue")
struct TORORUNTIME_API FWTNarrative_Dialogue final : public FWATask_Narrative
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		TSubclassOf<UDialogue> Dialogue;

	FWTNarrative_Dialogue() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Start Quest")
struct TORORUNTIME_API FWTNarrative_Quest final : public FWATask_Narrative
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		TSubclassOf<UQuest> Quest;

	FWTNarrative_Quest() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Complete Task")
struct TORORUNTIME_API FWTNarrative_Task final : public FWATask_Narrative
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		TObjectPtr<UNarrativeDataTask> Task;

	UPROPERTY(EditAnywhere, Category = Action)
		FString Argument;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ClampMin = 1, UIMin = 1))
		uint8 Quantity;

	FWTNarrative_Task(): Quantity(1)
	{
#if WITH_EDITOR
		Task = LoadObject<UNarrativeDataTask>(nullptr, TEXT("/Narrative/DefaultTasks/LooseTask.LooseTask"));
#endif
	}

protected:

	virtual void RunAction() override;
};
