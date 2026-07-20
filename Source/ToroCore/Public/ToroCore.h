// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroCore, All, All);

class FToroCoreModule final : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

public:

    static inline FString PLUGIN_NAME = TEXT("ToroUtilities");
};
