// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
