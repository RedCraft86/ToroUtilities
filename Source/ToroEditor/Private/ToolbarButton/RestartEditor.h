// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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