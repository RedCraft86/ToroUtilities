// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Libraries/ToroUtilityLibrary.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/Engine.h"
#include "Misc/App.h"

bool UToroUtilityLibrary::IsInGame()
{
#if WITH_EDITOR
	return FApp::IsGame();
#else
	return true;
#endif
}

void UToroUtilityLibrary::ForceGarbageCollection()
{
	if (GEngine)
	{
		GEngine->ForceGarbageCollection(IsInGame());
	}
}

int32 UToroUtilityLibrary::GetNumLoadingAsyncPkgs()
{
	return GetNumAsyncPackages();
}

void UToroUtilityLibrary::CallLocalEvent(UObject* Target, const FName EventName)
{
	if (Target && !EventName.IsNone())
	{
		FOutputDeviceNull Ar;
		Target->CallFunctionByNameWithArguments(*EventName.ToString(), Ar, nullptr, true);
	}
}
