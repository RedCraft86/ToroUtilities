// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Misc/MessageDialog.h"
#include "Helpers/ActorBaking.h"
#include "SlateStyle/ToroEditorStyle.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorSplitButton final : public FToroToolbarButton
{
public:

	FActorSplitButton()
		: FToroToolbarButton(MAKE_TOOLBAR_BUTTON_NAME(ActorSplit), INVTEXT("Actor Split"),
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
		const TArray<AActor*>& Actors = FActorBaking::GetSelectedActors();
		if (!Actors.IsEmpty())
		{
			if (FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Type::OkCancel,
				INVTEXT("This will gather mesh components from the selected actors and split them into individual static mesh actors."),
				INVTEXT("Are you sure you want to split selected meshes?")) == EAppReturnType::Cancel)
			{
				return;
			}

			const EAppReturnType::Type SrcRetType = FMessageDialog::Open(
				EAppMsgCategory::Warning, EAppMsgType::Type::YesNo,
				INVTEXT("Whether to destroy source actors, otherwise they will be kept."),
				INVTEXT("Do you want to destroy source actors?")
			);

			FActorBaking::SplitActors(Actors, SrcRetType == EAppReturnType::Yes);
		}
	}
};