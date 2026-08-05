// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Modules/ModuleManager.h"

TOROEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogToroEditor, All, All);

class FToroEditorModule final : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

public:

    static TOROEDITOR_API FString GetPluginDirectory();
};
