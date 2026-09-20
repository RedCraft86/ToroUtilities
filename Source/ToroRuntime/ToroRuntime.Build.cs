// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

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
		        "RenderCore",
		        "ApplicationCore",
		        "GameplayTags",
		        "EngineSettings",
		        "LevelSequence",
		        "MovieScene",
		        "CommonUI",
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