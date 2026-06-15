// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Modules/ModuleManager.h"

TOROCOMMONUI_API DECLARE_LOG_CATEGORY_EXTERN(LogToroCommonUI, All, All)

class FToroCommonUIModule final : public IModuleInterface
{
private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
