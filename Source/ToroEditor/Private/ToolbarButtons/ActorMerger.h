// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Editor.h"
#include "ToroEditorStyle.h"
#include "Framework/Docking/TabManager.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorMerger final : public FToroToolbarButton
{
public:

	FActorMerger()
		: FToroToolbarButton(TEXT("ActorMerger"), INVTEXT("Actor Merge"), 
			INVTEXT("Opens the Merge Actors window"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar");
		SectionName = TEXT("Actor");
	}

private:

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.ActorMerge"));
	}

	virtual void Execute() override
	{
		UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr;
		if (!Subsystem || (Subsystem && !Subsystem->GetSelectedLevelActors().IsEmpty()))
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FTabId(TEXT("MergeActors")));
		}
	}
};