// Copyright (C) RedCraft86. See LICENSE file for details.

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
                "Slate",
                "SlateCore"
	        ]
        );
    }
}