// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroActorLibrary.generated.h"

/**
 * Provides actor related functions.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroActorLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Calculates the scaled corners of an actor's world bounds. Invalid actors leave the outputs empty and zero.
	 * @param OutVerts Receives up to eight unique corner positions.
	 * @param Origin Receives the world bounds center.
	 * @param Extent Receives the unscaled world bounds half size.
	 * @param Target Actor whose bounds are queried.
	 * @param Scale Multiplier applied to the bounds extent on each axis.
	 * @param bOnlyColliding Whether to include only colliding components.
	 * @param bChildActors Whether to include attached child actors.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor, meta = (DefaultToSelf = Target))
		static void GetBoundingBoxVertices(TSet<FVector>& OutVerts, FVector& Origin, FVector& Extent, const AActor* Target,
			const FVector Scale = FVector(1.0f), const bool bOnlyColliding = false, const bool bChildActors = true);

	/**
	 * Adds a tag to a valid actor if it is not already present.
	 * @param Target Actor to update.
	 * @param InTag Tag to add.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Add Tag", meta = (DefaultToSelf = Target))
		static void AddActorTag(AActor* Target, const FName InTag);

	/**
	 * Removes a tag from a valid actor.
	 * @param Target Actor to update.
	 * @param InTag Tag to remove.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Remove Tag", meta = (DefaultToSelf = Target))
		static void RemoveActorTag(AActor* Target, const FName InTag);
};
