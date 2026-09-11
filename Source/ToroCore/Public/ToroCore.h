// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Modules/ModuleManager.h"

TOROCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogToroCore, All, All);

class FToroCoreModule final : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
