// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroEditor.h"
#include "ToroEditorStyle.h"

#include "ToolbarButton/RestartEditor.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"

#include "ComponentVis/FEdShapeVisualizer.h"
#include "ComponentVis/ComponentVisualizerRegistry.h"

#include "DetailsPanel/DetailsCustomizationRegistry.h"
#include "DetailsPanel/PropertyMetadataDetails.h"

#define LOCTEXT_NAMESPACE "ToroEditor"

DEFINE_LOG_CATEGORY(LogToroEditor)

void FToroEditorModule::StartupModule()
{
    FToroEditorStyle::Register();

    FToolbarButtonRegistry::AddButton<FRestartEditor>();
    FToolbarButtonRegistry::Register();

	FComponentVisualizerRegistry::Register<UEdShapeComponent, FEdShapeVisualizer>();

	if (FBlueprintEditorModule* BPEditorModule = FModuleManager::LoadModulePtr<FBlueprintEditorModule>(TEXT("Kismet")))
	{
		BPEditorModule->RegisterVariableCustomization(FProperty::StaticClass(),
			FOnGetVariableCustomizationInstance::CreateStatic(&FPropertyMetadataDetails::MakeInstance));
	}
}

void FToroEditorModule::ShutdownModule()
{
	if (FBlueprintEditorModule* BPEditorModule = FModuleManager::GetModulePtr<FBlueprintEditorModule>(TEXT("Kismet")))
	{
		const FDelegateHandle Handle;
		BPEditorModule->UnregisterVariableCustomization(FProperty::StaticClass(), Handle);
	}

	FToolbarButtonRegistry::UnregisterAll();
	FComponentVisualizerRegistry::UnregisterAll();
	FDetailsCustomizationRegistry::UnregisterAll();
    FToroEditorStyle::Unregister();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)