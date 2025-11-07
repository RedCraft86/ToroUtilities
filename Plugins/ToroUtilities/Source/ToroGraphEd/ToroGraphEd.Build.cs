// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

using UnrealBuildTool;

public class ToroGraphEd : ModuleRules
{
    public ToroGraphEd(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "UnrealEd",
                "Projects",
                "Engine",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "GraphEditor",
                "ApplicationCore",
                "AssetDefinition",
                "DeveloperSettings",
                "BlueprintGraph",
                
                "ToroCore",
                "ToroEditor",
                "ToroGraph"
            ]
        );
    }
}