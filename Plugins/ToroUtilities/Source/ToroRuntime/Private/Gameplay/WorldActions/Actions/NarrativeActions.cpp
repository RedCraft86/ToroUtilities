// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/WorldActions/Actions/NarrativeActions.h"
#include "Gameplay/Narrative/NarrativeManager.h"

void FWTNarrative_Dialogue::RunAction()
{
	if (!Dialogue) return;
	if (UNarrativeManager* Narrative = UNarrativeManager::Get(GetWorldContext()))
	{
		Narrative->BeginDialogue(Dialogue);
	}
}

void FWTNarrative_Quest::RunAction()
{
	if (!Quest) return;
	if (UNarrativeManager* Narrative = UNarrativeManager::Get(GetWorldContext()))
	{
		Narrative->BeginQuest(Quest);
	}
}

void FWTNarrative_Task::RunAction()
{
	if (!Task || Argument.IsEmpty() || Quantity == 0) return;
	if (UNarrativeManager* Narrative = UNarrativeManager::Get(GetWorldContext()))
	{
		Narrative->CompleteNarrativeDataTask(Task, Argument, Quantity);
	}
}
