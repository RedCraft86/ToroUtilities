// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Editor.h"
#include "ToroEditorStyle.h"
#include "Misc/MessageDialog.h"
#include "Helpers/ActorBaking.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorSplitter final : public FToroToolbarButton
{
public:

	FActorSplitter()
		: FToroToolbarButton(TEXT("ActorSplitter"), INVTEXT("Actor Split"), 
			INVTEXT("Splits mesh components from selected actors into individual static mesh actors"))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar");
		SectionName = TEXT("Actor");
	}

private:

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.ActorSplit"));
	}

	virtual void Execute() override
	{
		if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
		{
			const TArray<AActor*> Actors = Subsystem->GetSelectedLevelActors();
			if (Actors.IsEmpty()) return;

			if (FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Type::OkCancel,
				INVTEXT("This will gather mesh components from the selected actors and split them into individual static mesh actors."),
				INVTEXT("Are you sure you want to split selected meshes?")) == EAppReturnType::Cancel)
			{
				return;
			}

			const EAppReturnType::Type SrcRetType = 
				FMessageDialog::Open(EAppMsgCategory::Warning, EAppMsgType::Type::YesNo,
				INVTEXT("Whether to destroy source actors, otherwise they will be kept."),
				INVTEXT("Do you want to destroy source actors?"));

			FActorBaking::SplitActors(Actors, SrcRetType == EAppReturnType::Yes);
		}
	}
};