// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Modules/ModuleManager.h"

TORORUNTIME_API DECLARE_LOG_CATEGORY_EXTERN(LogToroRuntime, All, All);

class FToroRuntimeModule final : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
