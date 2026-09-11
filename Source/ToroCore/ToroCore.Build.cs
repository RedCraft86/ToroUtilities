// Copyright (C) RedCraft86. See LICENSE file for details.

using UnrealBuildTool;

public class ToroCore : ModuleRules
{
    public ToroCore(ReadOnlyTargetRules Target) : base(Target)
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