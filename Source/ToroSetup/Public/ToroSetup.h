// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogToroSetup, All, All);

class FToroSetupModule final : public IModuleInterface
{
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

public:

    static FString PickFileOrDir(const FString& StartDir, const FString& Title, const FString& FileTypes);
};
