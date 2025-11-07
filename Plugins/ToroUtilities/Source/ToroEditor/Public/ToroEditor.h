// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

#define REGISTER_CLASS_CUSTOMIZATION(Class, Customization) PropertyModule->RegisterCustomClassLayout(Class::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&Customization::MakeInstance));
#define UNREGISTER_CLASS_CUSTOMIZATION(Class) PropertyModule->UnregisterCustomClassLayout(Class::StaticClass()->GetFName());

#define REGISTER_STRUCT_CUSTOMIZATION_DIRECT(Property, Customization) PropertyModule->RegisterCustomPropertyTypeLayout(Property->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::MakeInstance));
#define UNREGISTER_STRUCT_CUSTOMIZATION_DIRECT(Property) PropertyModule->UnregisterCustomPropertyTypeLayout(Property->GetFName());

#define REGISTER_STRUCT_CUSTOMIZATION(Property, Customization) REGISTER_STRUCT_CUSTOMIZATION_DIRECT(Property::StaticStruct(), Customization)
#define UNREGISTER_STRUCT_CUSTOMIZATION(Property) UNREGISTER_STRUCT_CUSTOMIZATION_DIRECT(Property::StaticStruct())

#define REGISTER_STRUCT_CUSTOMIZATION_INHERITED(Type, Customization) \
    if (ScriptStruct->IsChildOf(Type::StaticStruct()) && !ScriptStruct->HasMetaData(TEXT("UniqueStructCustomization"))) \
    { REGISTER_STRUCT_CUSTOMIZATION_DIRECT(ScriptStruct, Customization) }

#define UNREGISTER_STRUCT_CUSTOMIZATION_INHERITED(Type) \
    if (ScriptStruct->IsChildOf(Type::StaticStruct()) && !ScriptStruct->HasMetaData(TEXT("UniqueStructCustomization"))) \
    { UNREGISTER_STRUCT_CUSTOMIZATION_DIRECT(ScriptStruct) }

#define REGISTER_VISUALIZER(Component, Visualizer) \
    TSharedPtr<FComponentVisualizer> VizInst_##Component = Visualizer::MakeInstance(); \
    GUnrealEd->RegisterComponentVisualizer(Component::StaticClass()->GetFName(), VizInst_##Component); \
    VizInst_##Component->OnRegister(); \

#define UNREGISTER_VISUALIZER(Component) \
    GUnrealEd->UnregisterComponentVisualizer(Component::StaticClass()->GetFName());

DECLARE_LOG_CATEGORY_EXTERN(LogToroEditor, All, All);

class FToroEditorModule final : public IModuleInterface
{
public:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:

    void RegisterMenus();
    TSharedPtr<FUICommandList> PluginCommands;
};

class TOROEDITOR_API FToroEditorStyle final
{
public:

    static void Init();
    static void Shutdown();
    static const ISlateStyle& Get() { return *StyleSet; }
    static const FName& GetStyleSetName() { return StyleSet->GetStyleSetName(); }

private:

    static TSharedPtr<FSlateStyleSet> StyleSet;
};
