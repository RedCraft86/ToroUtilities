// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

using UnrealBuildTool;

public class ToroEditor : ModuleRules
{
    public ToroEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            [
                "Core",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "Projects",
                "ToroCore",
                "ToroRuntime"
            ]
        );
    }
}