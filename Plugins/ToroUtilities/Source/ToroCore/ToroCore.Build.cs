// Copyright (C) RedCraft86. All Rights Reserved.

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