// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Modules/ModuleManager.h"

TOROCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogToroCore, All, All)

class FToroCoreModule final : public IModuleInterface
{
private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static void OnPreLoadMap(const FString& NewMap);
};
