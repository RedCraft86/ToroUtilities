// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
                "InputCore",
                "RenderCore",
                "GameplayTags",
                "NavigationSystem",
                "ProceduralMeshComponent", 
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