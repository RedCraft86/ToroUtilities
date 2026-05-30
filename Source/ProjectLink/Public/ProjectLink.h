// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Modules/ModuleManager.h"
#include "ToolbarButton/ToroToolbarButton.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProjectLink, All, All)

class FProjectLinkModule final : public IModuleInterface
{
public:

    static inline const class UProjectLinkSettings* Settings = nullptr;

private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override {}
};

class FProjectLinkButton final : public FToroToolbarButton
{
public:

    FProjectLinkButton()
        : FToroToolbarButton(TEXT("ProjectLink"), INVTEXT("Project Link"), 
            INVTEXT("Creates symlinks and copies specified configs to a target project"))
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

    static void ProcessSymlinks(const FString& SrcPath, const FString& DestPath);
    static void ProcessConfigs(const FString& SrcPath, const FString& DestPath);
};
