// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "ToroEditor.h"
#include "Interfaces/IPluginManager.h"
#include "SlateStyle/ToroEditorStyle.h"

#include "ToolbarButton/ToolbarButtonRegistry.h"
#include "ComponentVis/ComponentVisualizerRegistry.h"
#include "DetailsPanel/DetailsCustomizationRegistry.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define LOCTEXT_NAMESPACE "FToroEditorModule"

FString FToroEditorModule::GetPluginDir()
{
	static FString PluginDir = IPluginManager::Get().FindPlugin(UE_PLUGIN_NAME)->GetBaseDir();
	return PluginDir;
}

void FToroEditorModule::StartupModule()
{

}

void FToroEditorModule::ShutdownModule()
{
	FToolbarButtonRegistry::UnregisterAll();
	FComponentVisualizerRegistry::UnregisterAll();
	FDetailsCustomizationRegistry::UnregisterAll();
	FToroEditorStyle::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)