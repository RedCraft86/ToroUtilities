// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroEditor.h"
#include "ToroEditorStyle.h"

#include "ToolbarButton/RestartEditor.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"

#include "ComponentVis/ComponentVisualizerRegistry.h"

#define LOCTEXT_NAMESPACE "ToroEditor"

DEFINE_LOG_CATEGORY(LogToroEditor)

void FToroEditorModule::StartupModule()
{
    FToroEditorStyle::Register();

    FToolbarButtonRegistry::AddButton<FRestartEditor>();
    FToolbarButtonRegistry::Register();
}

void FToroEditorModule::ShutdownModule()
{
	FToolbarButtonRegistry::UnregisterAll();
	FComponentVisualizerRegistry::UnregisterAll();
    FToroEditorStyle::Unregister();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)