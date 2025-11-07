// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "NarrativeComponent.h"
#include "Widgets/QuestWidget.h"
#include "Widgets/DialogueWidget.h"
#include "Helpers/ClassGetterMacros.h"
#include "Framework/ToroPlayerState.h"
#include "NarrativeManager.generated.h"

UCLASS(NotBlueprintable, ClassGroup = (Game), meta = (BlueprintSpawnableComponent))
class TORORUNTIME_API UNarrativeManager : public UNarrativeComponent
{
	GENERATED_BODY()

public:

	UNarrativeManager() {}
	
	PLAYER_COMPONENT_GETTER(UNarrativeManager, AToroPlayerState, Narrative)

	UFUNCTION(BlueprintCallable, Category = Statics, meta = (WorldContext = "ContextObject"))
	static UNarrativeManager* GetNarrativeManager(const UObject* ContextObject, const int32 PlayerIdx = 0)
	{
		return Get(ContextObject, PlayerIdx);
	}

	UFUNCTION(BlueprintImplementableEvent)
		FText InjectTextVariables(const FText& InText) const;

	void ToggleQuests();
	virtual void DialogueBegan(UDialogue* Dialogue) override;
	virtual void DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue) override;
	virtual void DialogueLineStarted(UDialogue* Dialogue, UDialogueNode* Node, const FDialogueLine& DialogueLine) override;

private:

	TObjectPtr<AToroPlayerCharacter> PlayerChar;

	TObjectPtr<UQuestWidget> QuestWidget;
	UQuestWidget* GetQuestWidget();

	TObjectPtr<UDialogueWidget> DialogueWidget;
	UDialogueWidget* GetDialogueWidget();

	virtual void BeginPlay() override;
};
