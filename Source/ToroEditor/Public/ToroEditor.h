// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

TOROEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(LogToroEditor, All, All)

class TOROEDITOR_API FToroEditorModule final : public IModuleInterface
{
public:

    static FString GetPluginDirectory()
    {
        static FString Path = IPluginManager::Get().FindPlugin(TEXT("ToroUtilities"))->GetBaseDir();
        return Path;
    }

private:

    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
