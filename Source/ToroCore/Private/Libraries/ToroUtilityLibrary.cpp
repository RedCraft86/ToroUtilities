// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Libraries/ToroUtilityLibrary.h"
#include "Misc/OutputDeviceNull.h"

bool UToroUtilityLibrary::IsInGame()
{
#if WITH_EDITOR
	return FApp::IsGame();
#else
	return true;
#endif
}

FString UToroUtilityLibrary::GetProjectVersion()
{
	FString VersionStr;
	if (GConfig->GetString(TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"), VersionStr, GGameIni))
	{
		return VersionStr;
	}

	return TEXT("0.0.0");
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
