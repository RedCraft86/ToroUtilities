// Copyright (C) RedCraft86. See LICENSE file for details.

#pragma once

#include "Modules/ModuleManager.h"

TOROEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogToroEditor, All, All);

class FToroEditorModule final : public IModuleInterface
{
public:

	static FString GetPluginDir();

private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
