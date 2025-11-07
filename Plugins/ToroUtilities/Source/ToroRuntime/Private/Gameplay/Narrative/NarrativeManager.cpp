// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Narrative/NarrativeManager.h"
#include "Gameplay/Narrative/ToroDialogueNode_NPC.h"
#include "UserInterface/ToroWidgetManager.h"
#include "Framework/ToroPlayerCharacter.h"

void UNarrativeManager::ToggleQuests()
{
	if (UQuestWidget* Widget = GetQuestWidget())
	{
		Widget->ToggleVisibility();
	}
}

void UNarrativeManager::DialogueBegan(UDialogue* Dialogue)
{
	Super::DialogueBegan(Dialogue);
	if (UDialogueWidget* Widget = GetDialogueWidget())
	{
		Widget->PushWidget();
	}
}

void UNarrativeManager::DialogueFinished(UDialogue* Dialogue, const bool bStartingNewDialogue)
{
	Super::DialogueFinished(Dialogue, bStartingNewDialogue);
	if (UDialogueWidget* Widget = GetDialogueWidget())
	{
		Widget->PopWidget();
	}
	if (PlayerChar)
	{
		PlayerChar->ClearLockOnTarget();
	}
}

void UNarrativeManager::DialogueLineStarted(UDialogue* Dialogue, UDialogueNode* Node, const FDialogueLine& DialogueLine)
{
	Super::DialogueLineStarted(Dialogue, Node, DialogueLine);
	const UToroDialogueNode_NPC* NPCNode = Cast<UToroDialogueNode_NPC>(Node);
	if (AActor* LookTarget = NPCNode ? NPCNode->GetLookTarget() : nullptr)
	{
		PlayerChar->SetLockOnTarget(LookTarget);
	}
}

UQuestWidget* UNarrativeManager::GetQuestWidget()
{
	if (!QuestWidget)
	{
		QuestWidget = AToroWidgetManager::GetWidget<UQuestWidget>(this);
	}
	return QuestWidget;
}

UDialogueWidget* UNarrativeManager::GetDialogueWidget()
{
	if (!DialogueWidget)
	{
		DialogueWidget = AToroWidgetManager::GetWidget<UDialogueWidget>(this);
	}
	return DialogueWidget;
}

void UNarrativeManager::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		PlayerChar = AToroPlayerCharacter::Get(this);
	});
}
