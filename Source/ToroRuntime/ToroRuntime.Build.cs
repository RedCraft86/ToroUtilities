// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

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
				"SlateCore",
				"ToroCore",
				"GameplayTags"
			]
		);
	}
}