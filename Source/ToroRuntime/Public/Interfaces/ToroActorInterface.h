// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "UObject/Interface.h"
#include "DataTypes/ToroActorId.h"
#include "ToroActorInterface.generated.h"

/**
 * Reflection class for IToroActor.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UToroActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Provides a common identity and activation contract for Toro actors.
 */
class TORORUNTIME_API IToroActorInterface
{
	GENERATED_BODY()

public:

	/**
	 * Gets this actor's persistent identity.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Actor)
		FToroActorId GetIdentity() const;

	/**
	 * Gets whether this actor is active.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Actor)
		bool GetActiveState() const;

	/**
	 * Changes this actor's active state.
	 * @param bActive New active state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Actor)
		void SetActiveState(const bool bActive);

public:

	/**
	 * Safely gets an actor's persistent identity.
	 * @param Target Actor to query.
	 * @return EmptyId when the actor is invalid or does not implement this interface.
	 */
	static FToroActorId GetIdentity(const AActor* Target);

	/**
	 * Safely gets an actor's active state.
	 * @param Target Actor to query.
	 * @return False when the actor is invalid or does not implement this interface.
	 */
	static bool GetActiveState(const AActor* Target);

	/**
	 * Safely changes an actor's active state when it implements this interface.
	 * @param Target Actor to update.
	 * @param bActive New active state.
	 */
	static void SetActiveState(AActor* Target, const bool bActive);
};
