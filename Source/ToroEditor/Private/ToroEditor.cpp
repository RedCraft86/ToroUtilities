// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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

#include "DetailsPanel/AsyncMessageIdDetails.h"
#include "DetailsPanel/GameObjectIdDetails.h"
#include "DetailsPanel/PrimitiveCollisionDetails.h"
#include "DetailsPanel/SimpleCooldownDetails.h"
#include "DetailsPanel/WorldActionListDetails.h"
#include "DetailsPanel/InlineCurveDetails.h"
#include "DetailsPanel/WrappedTypeDetails.h"
#include "DetailsPanel/ToroClassCustomization.h"
#include "DetailsPanel/DetailsCustomizationRegistry.h"
#include "DetailsPanel/PropertyMetadataDetails.h"
#include "DetailsPanel/ToroWorldSettingsDetails.h"

#include "Actors/ToroActor.h"
#include "Actors/ToroVolume.h"
#include "Actors/ToroCharacter.h"
#include "MiscActors/TeleportPoint.h"
#include "LightProbes/LightProbeActor.h"

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

	FDetailsCustomizationRegistry::Register<FAsyncMessageId, FAsyncMessageIdDetails>();

	FDetailsCustomizationRegistry::Register<FGameObjectId, FGameObjectIdDetails>();
	FDetailsCustomizationRegistry::Register<FPrimitiveCollision, FPrimitiveCollisionDetails>();
	FDetailsCustomizationRegistry::Register<FSimpleCooldown, FSimpleCooldownDetails>();
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
	FDetailsCustomizationRegistry::Register<ALightProbeActor, FToroClassCustomization>();

	FDetailsCustomizationRegistry::Register<AToroWorldSettings, FToroWorldSettingsDetails>();

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