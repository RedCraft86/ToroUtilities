// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroUtilityLibrary.generated.h"

/**
 * Exposes runtime, class, and other utilities.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroUtilityLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Reports whether the current process is running in game mode.
	 */
	UFUNCTION(BlueprintPure, Category = Utility)
		static bool IsInGame();

	/**
	 * Gets the number of packages currently loading asynchronously.
	 */
	UFUNCTION(BlueprintPure, Category = Utility, DisplayName = "Get Num Async Packages")
		static int32 GetNumLoadingAsyncPkgs();

	/**
	 * Finds classes derived from a base class.
	 * @param OutClasses Receives matching classes.
	 * @param BaseClass Parent class to search.
	 * @param bRecursive Whether to include indirect descendants.
	 */
	UFUNCTION(BlueprintCallable, Category = Utility, DisplayName = "Get Derived Classes")
		static void GetChildClasses(TArray<UClass*>& OutClasses, const UClass* BaseClass, const bool bRecursive);

	/**
	 * Calls a named function on a valid object when available.
	 * @param Target Object receiving the call.
	 * @param EventName Function name to invoke without arguments.
	 */
	UFUNCTION(BlueprintCallable, Category = Object, meta = (DefaultToSelf = Target))
		static void CallObjectEvent(UObject* Target, const FName EventName);
};
