// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroEditor.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#include "ToroCommands.h"
#include "Toolbar/AssetLibrary.h"
#include "Toolbar/RestartEditor.h"
#include "Toolbar/ActorLayout.h"
#include "Toolbar/ActorMerger.h"
#include "Interfaces/IMainFrameModule.h"

#include "UnrealEd.h"
#include "ComponentVis/EditorShapeVisualizer.h"

#include "DetailsPanels/ToroCharacterDetails.h"
#include "DetailsPanels/ElectricActorDetails.h"
#include "DetailsPanels/ExprTextFieldsDetails.h"
#include "DetailsPanels/InlineCurveDetails.h"
#include "DetailsPanels/PrimitiveCollisionDetails.h"
#include "DetailsPanels/WorldActionArrayDetails.h"

#include "Actors/NavPathVisualizer.h"
#include "LevelZone/LevelZoneVolume.h"

DEFINE_LOG_CATEGORY(LogToroEditor);

#define LOCTEXT_NAMESPACE "ToroEditor"

void FToroEditorModule::StartupModule()
{
	FToroEditorStyle::Init();

	// Toolbar Buttons
	{
		FToroCommands::Register();
		PluginCommands = MakeShareable(new FUICommandList);

		FAssetLibrary::Register(PluginCommands);
		FRestartEditor::Register(PluginCommands);
		FActorLayout::Register(PluginCommands);
		FActorMerger::Register(PluginCommands);

		IMainFrameModule& MainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
		MainFrame.GetMainFrameCommandBindings()->Append(PluginCommands.ToSharedRef());
	
		UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &FToroEditorModule::RegisterMenus));
	}

	// Component Visualizers
	if (GUnrealEd)
	{
		REGISTER_VISUALIZER(UEditorShapeComponent, FEditorShapeVisualizer)
	}

	// Struct and Class Details Customization
	if (FPropertyEditorModule* PropertyModule = FModuleManager::LoadModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		REGISTER_CLASS_CUSTOMIZATION(AToroCharacter, FToroCharacterDetails)
		REGISTER_CLASS_CUSTOMIZATION(AElectricalActor, FElectricActorDetails)

		REGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve, FInlineCurveDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision, FPrimitiveCollisionDetails)
		REGISTER_STRUCT_CUSTOMIZATION(FWorldActionArray, FWorldActionArrayDetails)

		REGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields, FExprTextFieldsDetails)

		// for (TObjectIterator<UScriptStruct> It; It; ++It)
		// {
		// 	const UScriptStruct* ScriptStruct = *It;
		// 	if (!ScriptStruct) continue;
		// }
	}
}

void FToroEditorModule::ShutdownModule()
{
	// Toolbar Buttons
	{
		UToolMenus::UnRegisterStartupCallback(this);
		UToolMenus::UnregisterOwner(this);
		FToroCommands::Unregister();
	}

	// Component Visualizers
	if (GUnrealEd)
	{
		UNREGISTER_VISUALIZER(UEditorShapeComponent)
	}

	// Struct and Class Details Customization
	if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
	{
		UNREGISTER_CLASS_CUSTOMIZATION(AToroCharacter)
		UNREGISTER_CLASS_CUSTOMIZATION(AElectricalActor)

		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineFloatCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineVectorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FInlineColorCurve)
		UNREGISTER_STRUCT_CUSTOMIZATION(FPrimitiveCollision)
		UNREGISTER_STRUCT_CUSTOMIZATION(FWorldActionArray)

		UNREGISTER_STRUCT_CUSTOMIZATION(FExpressiveTextFields)

		// for (TObjectIterator<UScriptStruct> It; It; ++It)
		// {
		// 	const UScriptStruct* ScriptStruct = *It;
		// 	if (!ScriptStruct) continue;
		// }
	}

	FToroEditorStyle::Shutdown();
}

void FToroEditorModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		FAssetLibrary::RegisterMenus(PluginCommands);
		FRestartEditor::RegisterMenus(PluginCommands);
		FActorLayout::RegisterMenus(PluginCommands);
		FActorMerger::RegisterMenus(PluginCommands);
	}
}

#define RootToContentDir StyleSet->RootToContentDir
#define AddSVG(Name, ImageFile, Size) StyleSet->Set(#Name, new IMAGE_BRUSH_SVG(TEXT(#ImageFile), Icon##Size));
#define AddPNG(Name, ImageFile, Size) StyleSet->Set(#Name, new IMAGE_BRUSH(TEXT(#ImageFile), Icon##Size));
TSharedPtr<FSlateStyleSet> FToroEditorStyle::StyleSet = nullptr;
void FToroEditorStyle::Init()
{
	if (StyleSet.IsValid()) return;
	StyleSet = MakeShareable(new FSlateStyleSet("ToroEdStyle"));

	const FVector2D Icon64x64(64.0f, 64.0f);
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);

	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate/UMG"));
	
	AddSVG(ClassIcon.RadialProgressBar, ProgressBar, 16x16);

	const FString PluginDir = IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir();
	StyleSet->SetContentRoot(PluginDir / TEXT("Resources"));

	AddSVG(ToroEditor.AssetLibrary, LinkFolder, 20x20);
	AddSVG(ToroEditor.RestartEditor, RestartEditor, 20x20);
	AddSVG(ToroEditor.ActorLayout, ActorLayout, 20x20);
	AddSVG(ToroEditor.StaticMeshBaker, StaticMeshBaker, 20x20);
	AddSVG(ToroEditor.ActorMerger, ActorMerger, 20x20);

	AddSVG(ClassThumbnail.SplineCable, SplineCable, 64x64);
	AddSVG(ClassThumbnail.SplineBarrier, SplineBarrier, 64x64);
	AddPNG(ClassThumbnail.MasterPostProcess, MasterPostProcess, 64x64);
	AddPNG(ClassThumbnail.LevelZoneVolume, LevelZoneVolume, 64x64);
	AddPNG(ClassThumbnail.LightProbeActor, LightProbe, 64x64);

	AddSVG(ClassThumbnail.InventoryAsset, InventoryGrid, 64x64);
	AddSVG(ClassThumbnail.AchievementDatabase, Achievement, 64x64);
	AddSVG(ClassThumbnail.TutorialDatabase, Tutorial, 64x64);
	
	AddSVG(ToroEditor.InventoryGridSmall, InventoryGridSmall, 32x32);

	if (const TSharedPtr<IPlugin> ETPlugin = IPluginManager::Get().FindPlugin(TEXT("ExpressiveText")))
	{
		StyleSet->Set("ClassIcon.ExprTextBlock", new FSlateImageBrush(
			ETPlugin->GetContentDir() / TEXT("Core/Editor/Resources/ExTextLogo.png"),
			Icon16x16
		));
	}

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
}
#undef AddPNG
#undef AddSVG
#undef RootToContentDir

void FToroEditorStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroEditorModule, ToroEditor)