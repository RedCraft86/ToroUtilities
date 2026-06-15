// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
