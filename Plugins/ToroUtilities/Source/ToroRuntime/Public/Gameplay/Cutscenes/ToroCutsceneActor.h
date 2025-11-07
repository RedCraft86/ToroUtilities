// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "CutsceneWidget.h"
#include "LevelSequenceActor.h"
#include "Interfaces/GuidInterface.h"
#include "DataTypes/GameInputConfig.h"
#include "SaveSystem/ToroSaveManager.h"
#include "ToroCutsceneActor.generated.h"

UCLASS()
class TORORUNTIME_API AToroCutsceneActor final : public ALevelSequenceActor, public IGuidInterface
{
	GENERATED_BODY()

public:

	AToroCutsceneActor(const FObjectInitializer& Init);

	UFUNCTION(BlueprintCallable, Category = "Sequencer|Player")
		void PlayAfterFade(const float FadeTime = 0.5f);

	UFUNCTION(BlueprintCallable, Category = "Sequencer|Player")
		void PlayCutscene();

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Sequencer|Player")
		void SkipCutscene();

	DECLARE_MULTICAST_DELEGATE(FOnFinishedEvent)
	FOnFinishedEvent OnFinishedEvent;

	virtual FGuid GetUniqueGUID_Implementation() override { return CutsceneGuid; }

protected:

	UPROPERTY(EditAnywhere, Category = General)
		bool bSkippable;

	UPROPERTY(EditAnywhere, Category = General, NonPIEDuplicateTransient, TextExportTransient, NonTransactional)
		FGuid CutsceneGuid;

	FTimerHandle FadedPlayHandle;
	FGameInputConfig InputConfig;
	TObjectPtr<UToroSaveManager> SaveManager;

	TObjectPtr<UCutsceneWidget> CutsceneWidget;
	UCutsceneWidget* GetCutsceneWidget();

	void OnFinished();
	void LockPlayer();
	void UnlockPlayer();

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
