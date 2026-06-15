// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Editor.h"
#include "ToroEditorStyle.h"
#include "Misc/MessageDialog.h"
#include "Helpers/ActorInstancing.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "ToolbarButton/ToroToolbarButton.h"

class FActorInstancer final : public FToroToolbarButton
{
public:

	FActorInstancer()
		: FToroToolbarButton(TEXT("ActorInstancer"), INVTEXT("Actor Instancer"), 
			INVTEXT("Collects mesh components from selected actors and convert common ones to instanced static meshes while baking the ones that cannot be instanced."))
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
		if (UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr)
		{
			const TArray<AActor*> Actors = Subsystem->GetSelectedLevelActors();
			if (Actors.IsEmpty()) return;

			if (FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Type::OkCancel,
				INVTEXT("This will gather mesh components from the selected actors and convert common ones to instanced static meshes while baking the ones that cannot be instanced."),
				INVTEXT("Are you sure you want to Instance Selected Meshes?")) == EAppReturnType::Cancel)
			{
				return;
			}

			const EAppReturnType::Type HISMRetType = 
				FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Type::YesNo,
				INVTEXT("'Yes' to use Hierarchical Instanced Static Mesh over regular Instanced Static Mesh components."),
				INVTEXT("Do you want to use HISMs instead of ISMs?"));

			const EAppReturnType::Type SrcRetType = 
				FMessageDialog::Open(EAppMsgCategory::Warning, EAppMsgType::Type::YesNoCancel,
				INVTEXT("'Yes' to destroy source actors, otherwise they will be kept."),
				INVTEXT("Do you want to destroy source actors?"));

			if (SrcRetType == EAppReturnType::Cancel) return;
			FActorInstancing::InstanceActors(Actors, 
				HISMRetType == EAppReturnType::Yes, 
				SrcRetType == EAppReturnType::Yes
			);
		}
	}
};