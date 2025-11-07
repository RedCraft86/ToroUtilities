// Copyright (C) RedCraft86. All Rights Reserved.

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