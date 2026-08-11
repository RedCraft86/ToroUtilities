// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

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
				"UMG",
				"Slate",
				"SlateCore",
				"RenderCore",
				"InputCore",
				"ApplicationCore",
				"ProceduralMeshComponent",
				"NavigationSystem",
				"EngineSettings",
				"GameplayTags",
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