// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "ToroEditor.h"
#include "Interfaces/IPluginManager.h"

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
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)