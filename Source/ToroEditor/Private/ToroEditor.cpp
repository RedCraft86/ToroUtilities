// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#include "ToroEditor.h"
#include "Interfaces/IPluginManager.h"
#include "ToroCore.h"

DEFINE_LOG_CATEGORY(LogToroRuntime);

#define LOCTEXT_NAMESPACE "ToroEditor"

void FToroEditorModule::StartupModule()
{
}

void FToroEditorModule::ShutdownModule()
{
    
}

FString FToroEditorModule::GetPluginDirectory()
{
	static FString Path = IPluginManager::Get().FindPlugin(FToroCoreModule::PLUGIN_NAME)->GetBaseDir();
	return Path;
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)