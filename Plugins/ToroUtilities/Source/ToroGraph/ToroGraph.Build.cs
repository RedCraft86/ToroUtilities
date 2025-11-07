// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

using UnrealBuildTool;

public class ToroGraph : ModuleRules
{
    public ToroGraph(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                
                "ToroCore"
            ]
        );
    }
}