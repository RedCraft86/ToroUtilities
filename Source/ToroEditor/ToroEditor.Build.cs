// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

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
                "UMGEditor",
                "Blutility",
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