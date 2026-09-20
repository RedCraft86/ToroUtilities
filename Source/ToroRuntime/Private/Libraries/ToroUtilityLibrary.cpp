// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

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

int32 UToroUtilityLibrary::GetNumLoadingAsyncPkgs()
{
	return GetNumAsyncPackages();
}

void UToroUtilityLibrary::GetChildClasses(TArray<UClass*>& OutClasses, const UClass* BaseClass, const bool bRecursive)
{
	GetDerivedClasses(BaseClass, OutClasses, bRecursive);
}

void UToroUtilityLibrary::CallObjectEvent(UObject* Target, const FName EventName)
{
	if (IsValid(Target) && !EventName.IsNone())
	{
		FOutputDeviceNull Ar;
		Target->CallFunctionByNameWithArguments(*EventName.ToString(), Ar, nullptr, true);
	}
}
