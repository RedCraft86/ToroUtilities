// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
                "Projects",
                "UnrealEd",
                "Engine",
                "Slate",
                "SlateCore",
                "ToolMenus",
                "GraphEditor",
                "BlueprintGraph",
                "PropertyEditor",
                "ContentBrowser",
                "ContentBrowserData",
                "AssetDefinition",
                "DeveloperSettings",
                "EditorSubsystem",
                "ToroRuntime",
                "ToroCore"
            ]
        );
    }
}