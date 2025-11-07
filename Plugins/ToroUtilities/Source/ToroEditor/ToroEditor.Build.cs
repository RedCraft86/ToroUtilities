// Copyright (C) RedCraft86. All Rights Reserved.

using UnrealBuildTool;

public class ToroEditor : ModuleRules
{
    public ToroEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "UnrealEd",
                "Engine",
                "Slate",
                "SlateCore",
                "InputCore",
                "Projects",
                "MainFrame",
                "ToolMenus",
                "PropertyEditor",
                "AssetDefinition",
                "EditorSubsystem",
                "DeveloperSettings",
                "EditorScriptingUtilities",
                
                "ToroCore",
                "ToroRuntime",
                "ExpressiveText"
            ]
        );
    }
}