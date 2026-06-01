// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UnrealEdMisc.h"
#include "ToroEditorStyle.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FRestartEditor final : public FToroToolbarButton
{
public:

	FRestartEditor()
		: FToroToolbarButton(TEXT("RestartEditor"), INVTEXT("Restart Editor"), 
			INVTEXT("Prompts to save and restarts the Unreal Editor"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.ModesToolBar");
		SectionName = TEXT("File");
	}

private:

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.Restart"));
	}

	virtual void Execute() override
	{
		FUnrealEdMisc::Get().RestartEditor(false);
	}
};