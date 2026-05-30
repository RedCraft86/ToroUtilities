// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

TOROEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogToroEditor, All, All)

class TOROEDITOR_API FToroEditorModule final : public IModuleInterface
{
public:

    static FString GetPluginDirectory()
    {
        static FString Path = IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir();
        return Path;
    }

private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
