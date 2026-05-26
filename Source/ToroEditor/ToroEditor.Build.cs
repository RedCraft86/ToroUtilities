// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

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
                "Projects",
                "UnrealEd",
                "Engine",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "GraphEditor",
                "BlueprintGraph",
                "PropertyEditor",
                "ContentBrowser",
                "ContentBrowserData",
                "AssetDefinition",
                "DeveloperSettings",
                "EditorSubsystem",
                "ToroRuntime",
                "ToroCore"
            ]
        );
    }
}