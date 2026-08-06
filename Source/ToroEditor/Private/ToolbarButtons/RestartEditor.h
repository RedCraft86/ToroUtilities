// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

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
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.RestartEd"));
	}

	virtual void Execute() override
	{
		FUnrealEdMisc::Get().RestartEditor(false);
	}
};