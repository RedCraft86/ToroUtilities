// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

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
                "SlateCore",
                "GameplayTags"
            ]
        );
    }
}