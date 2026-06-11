// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Modules/ModuleManager.h"

TOROCOMMONUI_API DECLARE_LOG_CATEGORY_EXTERN(LogToroCommonUI, All, All)

class FToroCommonUIModule final : public IModuleInterface
{
private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
