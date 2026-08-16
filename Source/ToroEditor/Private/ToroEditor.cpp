// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "ToroEditor.h"
#include "Interfaces/IPluginManager.h"
#include "ToroEditorStyle.h"

#include "ToolbarButtons/RestartEditor.h"
#include "ToolbarButtons/ActorLayout.h"
#include "ToolbarButtons/ActorMerger.h"
#include "ToolbarButtons/ActorInstancer.h"
#include "ToolbarButtons/ActorSplitter.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"

#include "ComponentVis/EditorShapeVisualizer.h"
#include "ComponentVis/ComponentVisualizerRegistry.h"

#include "DetailsPanel/WrappedTypeDetails.h"
#include "DetailsPanel/InlineCurveDetails.h"
#include "DetailsPanel/GameObjectIdDetails.h"
#include "DetailsPanel/PrimitiveCollisionDetails.h"
#include "DetailsPanel/SimpleCooldownDetails.h"
#include "DetailsPanel/ToroDatabaseKeyDetails.h"
#include "DetailsPanel/AsyncMessageIdDetails.h"
#include "DetailsPanel/ToroDatabaseDetails.h"
#include "DetailsPanel/ToroActorBaseDetails.h"
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

	FComponentVisualizerRegistry::Register<UWireShapeComponent, FEditorShapeVisualizer>();

	FDetailsCustomizationRegistry::Register<FWrappedBool, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedFloat, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedByte, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedInt32, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedInt64, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedString, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FInlineFloatCurve, FInlineCurveDetails>();
	FDetailsCustomizationRegistry::Register<FInlineVectorCurve, FInlineCurveDetails>();
	FDetailsCustomizationRegistry::Register<FInlineColorCurve, FInlineCurveDetails>();
	FDetailsCustomizationRegistry::Register<FGameObjectId, FGameObjectIdDetails>();
	FDetailsCustomizationRegistry::Register<FPrimitiveCollision, FPrimitiveCollisionDetails>();
	FDetailsCustomizationRegistry::Register<FSimpleCooldown, FSimpleCooldownDetails>();
	FDetailsCustomizationRegistry::Register<FToroDatabaseKey, FToroDatabaseKeyDetails>();
	FDetailsCustomizationRegistry::Register<FAsyncMessageId, FAsyncMessageIdDetails>();

	FDetailsCustomizationRegistry::Register<UToroDatabase, FToroDatabaseDetails>();
	FDetailsCustomizationRegistry::Register<AToroActor, FToroActorCustomization>();
	FDetailsCustomizationRegistry::Register<AToroVolume, FToroVolumeCustomization>();
	FDetailsCustomizationRegistry::Register<AToroCharacter, FToroCharacterCustomization>();
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
	static FString Path = IPluginManager::Get().FindPlugin(UE_PLUGIN_NAME)->GetBaseDir();
	return Path;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)