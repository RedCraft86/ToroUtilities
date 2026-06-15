// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Editor.h"
#include "ToroEditorStyle.h"
#include "Misc/MessageDialog.h"
#include "Helpers/ActorBaking.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorBaker final : public FToroToolbarButton
{
public:

	FActorBaker()
		: FToroToolbarButton(TEXT("ActorBaker"), INVTEXT("Actor Bake"), 
			INVTEXT("Bakes mesh components from selected actors into individual static mesh actors"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar");
		SectionName = TEXT("Actor");
	}

private:

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.ActorBake"));
	}

	virtual void Execute() override
	{
		if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
		{
			const TArray<AActor*> Actors = Subsystem->GetSelectedLevelActors();
			if (Actors.IsEmpty()) return;

			if (FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Type::OkCancel,
				INVTEXT("This will gather mesh components from the selected actors and bake them into individual static mesh actors."),
				INVTEXT("Are you sure you want to Bake Selected Meshes?")) == EAppReturnType::Cancel)
			{
				return;
			}

			const EAppReturnType::Type SrcRetType = 
				FMessageDialog::Open(EAppMsgCategory::Warning, EAppMsgType::Type::YesNoCancel,
				INVTEXT("'Yes' to destroy source actors, otherwise they will be kept."),
				INVTEXT("Do you want to destroy source actors?"));

			if (SrcRetType == EAppReturnType::Cancel) return;
			FActorBaking::BakeActors(Actors, SrcRetType == EAppReturnType::Yes);
		}
	}
};