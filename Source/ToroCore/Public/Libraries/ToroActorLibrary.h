// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroActorLibrary.generated.h"

/**
 * A utility library providing functions for game actors.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroActorLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Calculates the bounding box vertices of an actor in world space.
	 * @param OutVerts The set of 8 world-space vertices forming the box.
	 * @param Origin The center point of the bounding box.
	 * @param Extent The half-size dimensions of the box.
	 * @param Target The actor to analyze.
	 * @param Scale Optional scale multiplier for the resulting box.
	 * @param bOnlyColliding If true, only components with collision enabled contribute to the box.
	 * @param bChildActors If true, includes the bounding boxes of all child actors in the calculation.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor, meta = (DefaultToSelf = "Target"))
		static void GetBoundingBoxVertices(TSet<FVector>& OutVerts, FVector& Origin, FVector& Extent, const AActor* Target,
			const FVector Scale = FVector(1.0f), const bool bOnlyColliding = false, const bool bChildActors = true);

	/**
	 * Adds a unique Tag to the actor's Tags array.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Add Tag", meta = (DefaultToSelf = "Target"))
		static void AddActorTag(AActor* Target, const FName InTag);

	/**
	 * Removes a specific Tag from the actor's Tags array.
	 */
	UFUNCTION(BlueprintCallable, Category = Actor, DisplayName = "Remove Tag", meta = (DefaultToSelf = "Target"))
		static void RemoveActorTag(AActor* Target, const FName InTag);
};
