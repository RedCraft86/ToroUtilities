// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "ToroCore.h"

DEFINE_LOG_CATEGORY(LogToroCore);

#define LOCTEXT_NAMESPACE "ToroCore"

void FToroCoreModule::StartupModule()
{

}

void FToroCoreModule::ShutdownModule()
{

}

FString FToroCoreModule::GetPluginName()
{
    static FString Name = TEXT("ToroUtilities");
    return Name;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FToroCoreModule, ToroCore)