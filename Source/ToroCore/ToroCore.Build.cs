// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

using UnrealBuildTool;

public class ToroCore : ModuleRules
{
    public ToroCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "InputCore",
                "RenderCore",
                "CommonUI",
                "GameplayTags",
                "MovieScene",
                "LevelSequence",
                "NavigationSystem",
                "ProceduralMeshComponent",
                "ExpressiveText"
            ]
        );
        
        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
                [
                    "UnrealEd",
                    "EditorScriptingUtilities"
                ]
            );
        }
    }
}