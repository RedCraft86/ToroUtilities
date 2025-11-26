// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "QuestSM.h"
#include "ToroRuntime.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "UserInterface/ToroManagedWidget.h"
#include "QuestWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UQuestWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UQuestWidget(const FObjectInitializer& ObjectInitializer);

	void ToggleVisibility();

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> QuestLabel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UVerticalBox> BranchContainer;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FText TaskPrefix;

	UPROPERTY(EditDefaultsOnly, Category = Settings)
		FSlateFontInfo TaskFont;

	UPROPERTY(Transient)
		TMap<TObjectPtr<const UQuestBranch>, TObjectPtr<UVerticalBox>> BranchBoxes;

	bool bVisible;
	TObjectPtr<class UNarrativeManager> Manager;

	UFUNCTION() void OnQuestNewState(UQuest* Quest, const UQuestState* NewState);
	UFUNCTION() void OnQuestTaskCompleted(const UQuest* Quest, const UNarrativeTask* CompletedTask, const UQuestBranch* Branch);
	UFUNCTION() void OnQuestTaskProgressChanged(const UQuest* Quest, const UNarrativeTask* ProgressedTask, const UQuestBranch* Branch, int32 OldProgress, int32 NewProgress);
	UFUNCTION() void OnQuestSucceeded(const UQuest* Quest, const FText& QuestSucceededMessage);

	void ConfigureVisibility();
	void ShowObjectiveNotice() const;
	void AddOrUpdateQuestBranch(const UQuestBranch* Branch);
	UVerticalBox* FindOrAddTaskContainer(const UQuestBranch* Branch);
	void ClearQuestContainers(const UQuest* Quest);

	virtual void NativeConstruct() override;
	virtual bool ShouldHideWidget() const override;
	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
