// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

using UnrealBuildTool;

public class ToroRuntime : ModuleRules
{
    public ToroRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

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
                "PhysicsCore",
                "MovieScene",
                "LevelSequence",
                "GameplayTags",
                "AudioExtensions",
                "DeveloperSettings",
                "ProceduralMeshComponent",
                "MetasoundFrontend",
                "MetasoundEngine",
                "CommonUI",
                "ToroCommonUI",
                "ToroCore",
                "UE5Coro"
            ]
        );

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
                [
                    "UnrealEd"
                ]
            );
        }
    }
}