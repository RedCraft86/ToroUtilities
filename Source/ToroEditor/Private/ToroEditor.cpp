// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "ToroEditor.h"
#include "ToroCore.h"
#include "Interfaces/IPluginManager.h"
#include "ToroEditorStyle.h"

#include "ToolbarButtons/RestartEditor.h"
#include "ToolbarButtons/ActorLayout.h"
#include "ToolbarButtons/ActorMerger.h"
#include "ToolbarButtons/ActorInstancer.h"
#include "ToolbarButtons/ActorSplitter.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"

#include "ComponentVis/ComponentVisualizerRegistry.h"

#include "DetailsPanel/DetailsCustomizationRegistry.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define LOCTEXT_NAMESPACE "ToroEditor"

void FToroEditorModule::StartupModule()
{
    FToroEditorStyle::Register();

	FToolbarButtonRegistry::AddButton<FRestartEditor>();
	FToolbarButtonRegistry::AddButton<FActorLayout>();
	FToolbarButtonRegistry::AddButton<FActorMerger>();
	FToolbarButtonRegistry::AddButton<FActorInstancer>();
	FToolbarButtonRegistry::AddButton<FActorSplitter>();
    FToolbarButtonRegistry::Register();
}

void FToroEditorModule::ShutdownModule()
{
	FToolbarButtonRegistry::UnregisterAll();
	FComponentVisualizerRegistry::UnregisterAll();
	FDetailsCustomizationRegistry::UnregisterAll();
    FToroEditorStyle::Unregister();
}

FString FToroEditorModule::GetPluginDirectory()
{
	static FString Path = IPluginManager::Get().FindPlugin(FToroCoreModule::GetPluginName())->GetBaseDir();
	return Path;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)