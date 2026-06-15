// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroUtilityLibrary.generated.h"

/**
 * A collection of miscellaneous engine-level utilities.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroUtilityLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Checks if the engine is currently running in a game context (Standalone, PIE, or Packaged).
	 * @return True if the code is executing within a game instance rather than just the Editor.
	 */
	UFUNCTION(BlueprintPure, Category = Misc)
		static bool IsInGame();

	/** 
	 * Retrieves the number of packages currently being loaded asynchronously.
	 * Useful for creating custom progress bars or waiting for background asset loading.
	 */
	UFUNCTION(BlueprintPure, Category = World, DisplayName = "Get Num Async Packages")
		static int32 GetNumLoadingAsyncPkgs();

	/** 
	 * Dynamically searches for and calls a UFUNCTION by name on a specific object.
	 * 
	 * @param Target      The object on which to search for the function.
	 * @param EventName   The exact name of the function/event to execute.
	 */
	UFUNCTION(BlueprintCallable, Category = Object, meta = (DefaultToSelf = "Target"))
		static void CallLocalEvent(UObject* Target, const FName EventName);
};
