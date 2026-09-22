// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Helpers/ActorBaking.h"
#include "SlateStyle/ToroEditorStyle.h"
#include "Framework/Docking/TabManager.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorMergeButton final : public FToroToolbarButton
{
public:

	FActorMergeButton()
		: FToroToolbarButton(MAKE_TOOLBAR_BUTTON_NAME(ActorMerge), INVTEXT("Actor Merge"),
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
		if (!FActorBaking::GetSelectedActors().IsEmpty())
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FTabId(TEXT("MergeActors")));
		}
	}
};