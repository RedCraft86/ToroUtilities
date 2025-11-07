// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "NativeWidgets/NoticeWidget.h"
#include "NativeWidgets/SubtitleWidget.h"
#include "Gameplay/WorldActions/WorldActionBase.h"
#include "MiscActions.generated.h"

USTRUCT(BlueprintInternalUseOnly, DisplayName = "Misc", meta = (Hidden))
struct TORORUNTIME_API FWATask_Misc : public FWorldActionBase
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType, DisplayName = "Stream Level")
struct TORORUNTIME_API FWTMisc_LevelStreaming final : public FWATask_Misc
{
	GENERATED_BODY()

	/* (Level, MakeVisibleAfterLoad) */
	UPROPERTY(EditAnywhere, Category = Action)
		TMap<TSoftObjectPtr<UWorld>, bool> Load;

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<UWorld>> Unload;

	FWTMisc_LevelStreaming() {}

protected:

	virtual void RunAction() override;
	virtual void OnBeginPlay() override;
};

USTRUCT(BlueprintType, DisplayName = "Step Phase")
struct TORORUNTIME_API FWTMisc_StepPhase final : public FWATask_Misc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		uint8 Index;

	FWTMisc_StepPhase(): Index(0) {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Unlock Content")
struct TORORUNTIME_API FWTMisc_Content final : public FWATask_Misc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		TSet<FGameplayTag> Content;

	FWTMisc_Content() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Show Tutorial")
struct TORORUNTIME_API FWTMisc_Tutorial final : public FWATask_Misc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "Tutorial"))
		TArray<FGameplayTag> Tutorials;

	FWTMisc_Tutorial() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Show Subtitle")
struct TORORUNTIME_API FWTMisc_Subtitle final : public FWATask_Misc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		TArray<FToroSubtitle> Subtitles;

	FWTMisc_Subtitle() {}

protected:

	virtual void RunAction() override;
};

USTRUCT(BlueprintType, DisplayName = "Show Notice")
struct TORORUNTIME_API FWTMisc_Notice final : public FWATask_Misc
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Action)
		bool bTitle;

	UPROPERTY(EditAnywhere, Category = Action)
		bool bImmediate;

	UPROPERTY(EditAnywhere, Category = Action, meta = (ShowOnlyInnerProperties = true))
		FToroSimpleMsg Message;

	FWTMisc_Notice(): bTitle(false), bImmediate(false) {}

protected:

	virtual void RunAction() override;
};
