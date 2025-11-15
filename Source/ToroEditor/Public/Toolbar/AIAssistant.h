// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToroCommands.h"
#include "ToroEditorSettings.h"
#include "EditorUtilitySubsystem.h"
#include "Framework/Commands/Commands.h"

class FAIAssistant
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(AIAssistant, FAIAssistant);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(AIAssistant)
		REGISTER_TOOLBAR(AIAssistant, General)
	}

private:

	static void ExecuteAction()
	{
		if (UEditorUtilityWidgetBlueprint* Widget = UToroEditorSettings::Get()->AIAssistantWidget.LoadSynchronous())
		{
			if (UEditorUtilitySubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>())
			{
				Subsystem->SpawnAndRegisterTab(Widget);
			}
		}
	}
};