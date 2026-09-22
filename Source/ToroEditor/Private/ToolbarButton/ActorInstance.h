// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Misc/MessageDialog.h"
#include "Helpers/ActorBaking.h"
#include "SlateStyle/ToroEditorStyle.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorInstanceButton final : public FToroToolbarButton
{
public:

	FActorInstanceButton()
		: FToroToolbarButton(MAKE_TOOLBAR_BUTTON_NAME(ActorInstance), INVTEXT("Actor Instance"),
			INVTEXT("Collects mesh components from selected actors and convert common ones to instanced static meshes while splitting the ones that cannot be instanced."))
	{
		MenuHook = TEXT("LevelEditor.LevelEditorToolBar.PlayToolBar");
		SectionName = TEXT("Actor");
	}

private:

	virtual FSlateIcon GetSlateIcon() override
	{
		return FSlateIcon(FToroEditorStyle::GetName(), TEXT("Toolbar.ActorInstance"));
	}

	virtual void Execute() override
	{
		const TArray<AActor*>& Actors = FActorBaking::GetSelectedActors();
		if (!Actors.IsEmpty())
		{
			if (FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Type::OkCancel,
				INVTEXT("This will gather mesh components from the selected actors and convert common ones to instanced static meshes while baking the ones that cannot be instanced."),
				INVTEXT("Are you sure you want to instance selected meshes?")) == EAppReturnType::Cancel)
			{
				return;
			}

			const EAppReturnType::Type HISMRetType = FMessageDialog::Open(
				EAppMsgCategory::Info, EAppMsgType::Type::YesNo,
				INVTEXT("Whether to use Hierarchical Instanced Static Mesh over regular Instanced Static Mesh components."),
				INVTEXT("Do you want to use HISMs instead of ISMs?")
			);

			const EAppReturnType::Type SrcRetType = FMessageDialog::Open(
				EAppMsgCategory::Warning, EAppMsgType::Type::YesNo,
				INVTEXT("Whether to destroy source actors, otherwise they will be kept."),
				INVTEXT("Do you want to destroy source actors?")
			);

			FActorBaking::InstanceActors(Actors,
				HISMRetType == EAppReturnType::Yes,
				SrcRetType == EAppReturnType::Yes
			);
		}
	}
};