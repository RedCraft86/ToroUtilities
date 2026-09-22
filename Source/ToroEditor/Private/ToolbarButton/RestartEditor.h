// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "UnrealEdMisc.h"
#include "SlateStyle/ToroEditorStyle.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FRestartEditorButton final : public FToroToolbarButton
{
public:

	FRestartEditorButton()
		: FToroToolbarButton(MAKE_TOOLBAR_BUTTON_NAME(RestartEditor), INVTEXT("Restart Editor"),
			INVTEXT("Prompts to save and restarts the Unreal Editor"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.ModesToolBar");
		SectionName = TEXT("File");
	}

private:

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.RestartEd"));
	}

	virtual void Execute() override
	{
		FUnrealEdMisc::Get().RestartEditor(false);
	}
};
