// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroEditor.h"
#include "ToroEditorStyle.h"

#include "ToolbarButton/RestartEditor.h"
#include "ToolbarButton/ActorLayout.h"
#include "ToolbarButton/ActorMerger.h"
#include "ToolbarButton/ActorBaker.h"
#include "ToolbarButton/ActorInstancer.h"
#include "ToolbarButton/ToolbarButtonRegistry.h"

#include "ComponentVis/FEdShapeVisualizer.h"
#include "ComponentVis/ComponentVisualizerRegistry.h"

#include "DetailsPanel/GameObjectIdDetails.h"
#include "DetailsPanel/PrimitiveCollisionDetails.h"
#include "DetailsPanel/WorldActionListDetails.h"
#include "DetailsPanel/InlineCurveDetails.h"
#include "DetailsPanel/WrappedTypeDetails.h"
#include "DetailsPanel/ToroClassCustomization.h"
#include "DetailsPanel/DetailsCustomizationRegistry.h"
#include "DetailsPanel/PropertyMetadataDetails.h"

#include "Actors/ToroActor.h"
#include "Actors/ToroCharacter.h"
#include "Actors/ToroVolume.h"
#include "MiscActors/TeleportPoint.h"

#define LOCTEXT_NAMESPACE "ToroEditor"

DEFINE_LOG_CATEGORY(LogToroEditor)

void FToroEditorModule::StartupModule()
{
    FToroEditorStyle::Register();

	FToolbarButtonRegistry::AddButton<FRestartEditor>();
	FToolbarButtonRegistry::AddButton<FActorLayout>();
	FToolbarButtonRegistry::AddButton<FActorMerger>();
	FToolbarButtonRegistry::AddButton<FActorBaker>();
	FToolbarButtonRegistry::AddButton<FActorInstancer>();
    FToolbarButtonRegistry::Register();

	FComponentVisualizerRegistry::Register<UEdShapeComponent, FEdShapeVisualizer>();

	FDetailsCustomizationRegistry::Register<FGameObjectId, FGameObjectIdDetails>();
	FDetailsCustomizationRegistry::Register<FPrimitiveCollision, FPrimitiveCollisionDetails>();
	FDetailsCustomizationRegistry::Register<FWorldActionList, FWorldActionListDetails>();

	FDetailsCustomizationRegistry::Register<FInlineFloatCurve, FInlineCurveDetails>();
	FDetailsCustomizationRegistry::Register<FInlineVectorCurve, FInlineCurveDetails>();
	FDetailsCustomizationRegistry::Register<FInlineColorCurve, FInlineCurveDetails>();

	FDetailsCustomizationRegistry::Register<FWrappedBool, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedFloat, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedByte, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedInt32, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedInt64, FWrappedTypeDetails>();
	FDetailsCustomizationRegistry::Register<FWrappedString, FWrappedTypeDetails>();

	FDetailsCustomizationRegistry::Register<AToroActor, FToroClassCustomization>();
	FDetailsCustomizationRegistry::Register<AToroVolume, FToroClassCustomization>();
	FDetailsCustomizationRegistry::Register<AToroCharacter, FToroClassCustomization>();
	FDetailsCustomizationRegistry::Register<ATeleportPoint, FToroClassCustomization>();

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