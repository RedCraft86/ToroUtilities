// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "UObject/Interface.h"
#include "ToroExitInterface.generated.h"

/**
 * Reflection class for IToroExitable.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UToroExitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Allows an object to accept or reject an exit request.
 */
class TORORUNTIME_API IToroExitInterface
{
	GENERATED_BODY()

public:

	/**
	 * Request to exit or close this object.
	 * @param Instigator Object that initiated the request.
	 * @return True if the exit request was accepted.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Misc)
		bool RequestExit(const UObject* Instigator);

	/**
	 * Safely sends an exit request to an object that implements this interface.
	 * @param Target Object receiving the request.
	 * @param Instigator Object that initiated the request.
	 * @return False when the object is invalid, unsupported, or rejects the request.
	 */
	static bool RequestExit(UObject* Target, const UObject* Instigator);
};
