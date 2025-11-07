// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroCommands.h"
#include "Framework/Commands/Commands.h"

class FRestartEditor
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(RestartEditor, FRestartEditor);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(RestartEditor)
		REGISTER_TOOLBAR(RestartEditor, General)
	}

private:

	static void ExecuteAction()
	{
		FUnrealEdMisc::Get().RestartEditor(false);
	}
};