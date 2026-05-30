// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Modules/ModuleManager.h"

TORORUNTIME_API DECLARE_LOG_CATEGORY_EXTERN(LogToroRuntime, All, All)

class FToroRuntimeModule final : public IModuleInterface
{
private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
