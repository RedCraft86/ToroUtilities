// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

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
	 * Manually triggers the Garbage Collector.
	 * @warning This is a heavy operation and should be used sparingly, typically during loading screens.
	 */
	UFUNCTION(BlueprintCallable, Category = World)
		static void ForceGarbageCollection();

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
