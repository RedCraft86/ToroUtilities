// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroUtilityLibrary.generated.h"

/**
 * A utility library providing functions that don't quite fit other categories.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroUtilityLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Checks whether this is currently running as a game or a packaged application, and is not acting like an editor.
	 * @return True if a normal or PIE game is active, including a dedicated server. False if called by editor code.
	 */
	UFUNCTION(BlueprintPure, Category = Misc)
		static bool IsInGame();

	/**
	 * Gets the version label of this project specified in the Project Settings.
	 * @return The project version label.
	 */
	UFUNCTION(BlueprintPure, Category = Misc)
		static FString GetProjectVersion();

	/**
	 * Retrieves the number of packages currently being loaded asynchronously.
	 * Useful for creating custom progress bars or waiting for background asset loading.
	 */
	UFUNCTION(BlueprintPure, Category = World, DisplayName = "Get Num Async Packages")
		static int32 GetNumLoadingAsyncPkgs();

	/**
	 * Calls a UFUNCTION by name on the specified object.
	 * @warning Generally not recommended, use as last resort.
	 * @param Target The object in which the function/event is located.
	 * @param EventName The name of the function/event to execute.
	 */
	UFUNCTION(BlueprintCallable, Category = Object, meta = (DefaultToSelf = "Target"))
		static void CallLocalEvent(UObject* Target, const FName EventName);
};
