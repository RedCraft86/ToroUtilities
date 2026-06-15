// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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