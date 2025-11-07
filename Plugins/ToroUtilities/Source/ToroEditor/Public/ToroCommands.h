// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"

#define MAP_TOOL(Tool, ThisClass) \
	Commands->MapAction(FToroCommands::Get().Tool, FExecuteAction::CreateStatic(&ThisClass::ExecuteAction), FCanExecuteAction());

#define REGISTER_TOP_MENU(Tool) \
	if (UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools")) \
	{ \
		FToolMenuSection& Section = Menu->FindOrAddSection("ToroEditor", INVTEXT("Toro Utilities")); \
		Section.AddMenuEntry(FToroCommands::Get().Tool); \
	}

#define REGISTER_TOOLBAR(Tool, Category) \
	if (UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar")) \
	{ \
		FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Toolbar."#Category); \
		Section.AddEntry(FToolMenuEntry::InitToolBarButton(FToroCommands::Get().Tool)).SetCommandList(Commands); \
	}

#define LOCTEXT_NAMESPACE "ToroEditor"
class FToroCommands final : public TCommands<FToroCommands>
{
public:

	FToroCommands()
		: TCommands(TEXT("ToroEditor"), LOCTEXT("Contexts", "ToroEditor"),
			NAME_None, FToroEditorStyle::GetStyleSetName())
	{}

	virtual void RegisterCommands() override
	{
		UI_COMMAND(AssetLibrary, "Link Asset Library", "Links a shared asset library (Specified in ToroEditor settings)",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(RestartEditor, "Restart Editor", "Restarts the Unreal Editor",
			EUserInterfaceActionType::Button, FInputChord(EModifierKey::Control | EModifierKey::Shift, EKeys::R));

		UI_COMMAND(ActorLayout, "Actor Layout", "Lay out selected actors in a Overview style",
			EUserInterfaceActionType::Button, FInputChord());

		UI_COMMAND(ActorMerger, "Merge Actors", "Opens the Merge Actors tab",
			EUserInterfaceActionType::Button, FInputChord());
	}

	TSharedPtr<FUICommandInfo> AssetLibrary;
	TSharedPtr<FUICommandInfo> RestartEditor;
	TSharedPtr<FUICommandInfo> ActorLayout;
	TSharedPtr<FUICommandInfo> ActorMerger;
};
#undef LOCTEXT_NAMESPACE