// Copyright (C) RedCraft86. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ToroRuntime : ModuleRules
{
    public ToroRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseUnity = false;

        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/Gameplay"));
        
        // Fixes a really weird error that honestly makes no sense in this context
        // MovieSceneNumericVariant.h... not enough arguments for function-like macro invocation 'max'...
        // Moved to SaveSystem/ToroSaveTypes.h
        // if (!PrivateDefinitions.Contains("NOMINMAX"))
        // {
        //     PrivateDefinitions.Add("NOMINMAX");
        // }

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "RenderCore",
                "Engine",
                "UMG",
                "Slate",
                "SlateCore",
                "EnhancedInput",
                "InputCore",
                "CommonUI",
                "MovieScene",
                "LevelSequence",
                "PhysicsCore",
                "GameplayTags",
                "EngineSettings",
                "MetasoundEngine",
                "AudioExtensions",
                "DeveloperSettings",
                
                "ToroCore",
                "ToroGraph",
                "Narrative",
                "EnhancedCodeFlow",
                "ExpressiveText",
                
                "XeFGRHI",
                "XeSSBlueprint",
                "XeFGBlueprint",
                "XeLLBlueprint",
                
                "DLSSBlueprint",
                "StreamlineBlueprint",
                "StreamlineDLSSGBlueprint",
                "StreamlineReflexBlueprint"
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