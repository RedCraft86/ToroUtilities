// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "ToroEditor.h"
#include "Interfaces/IPluginManager.h"
#include "ToroCore.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define LOCTEXT_NAMESPACE "ToroEditor"

void FToroEditorModule::StartupModule()
{
}

void FToroEditorModule::ShutdownModule()
{

}

FString FToroEditorModule::GetPluginDirectory()
{
	static FString Path = IPluginManager::Get().FindPlugin(FToroCoreModule::GetPluginName())->GetBaseDir();
	return Path;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)