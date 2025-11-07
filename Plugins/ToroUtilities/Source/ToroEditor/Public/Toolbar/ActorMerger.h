// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCommands.h"
#include "Framework/Commands/Commands.h"
#include "Subsystems/EditorActorSubsystem.h"

class FActorMerger
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(ActorMerger, FActorMerger);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(ActorMerger)
		REGISTER_TOOLBAR(ActorMerger, Actor)
	}

private:

	static void ExecuteAction()
	{
		UEditorActorSubsystem* Subsystem = GEditor ? GEditor->GetEditorSubsystem<UEditorActorSubsystem>() : nullptr;
		if (!Subsystem || (Subsystem && !Subsystem->GetSelectedLevelActors().IsEmpty()))
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FTabId("MergeActors"));
		}
	}
};