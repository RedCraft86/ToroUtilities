// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroCore.h"
#include "Helpers/WorldGetter.h"

#define LOCTEXT_NAMESPACE "ToroCore"

DEFINE_LOG_CATEGORY(LogToroCore)

void FToroCoreModule::StartupModule()
{
	FCoreUObjectDelegates::PreLoadMap.AddStatic(&FToroCoreModule::OnPreLoadMap);
}

void FToroCoreModule::ShutdownModule()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FWorldGetter::Reset();
}

void FToroCoreModule::OnPreLoadMap(const FString& NewMap)
{
	FWorldGetter::Reset();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToroCoreModule, ToroCore)