// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Modules/ModuleManager.h"
#include "ToolbarButton/ToroToolbarButton.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectLink, All, All)

class FProjectLinkModule final : public IModuleInterface
{
public:

    static inline const class UProjectLinkSettings* Settings = nullptr;
    static FString PickFileOrDir(const FString& StartDir, const FString& Title, const FString& FileTypes);

private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override {}
};

class FProjectLinkButton final : public FToroToolbarButton
{
public:

    FProjectLinkButton()
        : FToroToolbarButton(TEXT("ProjectLink"), INVTEXT("Project Link"), 
            INVTEXT("Creates symlinks, copies specified base configs, and uproject plugins to a target project"))
    {
        MenuHook = TEXT("LevelEditor.LevelEditorToolBar.ModesToolBar");
        SectionName = TEXT("File");
    }

private:

    virtual FSlateIcon GetSlateIcon() override
    {
        return FSlateIcon(FCoreStyle::Get().GetStyleSetName(), TEXT("Icons.Toolbar.Export"));
    }

    virtual void Execute() override;

    static void ProcessSymlinks(const FString& SrcDir, const FString& DestDir);
    static void ProcessConfigs(const FString& SrcDir, const FString& DestDir);
    static void ProcessPlugins(const FString& SrcFile, const FString& DestFile);
};
