// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroGeneralUtils.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FActorBoundsCheckParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		TEnumAsByte<ECollisionChannel> LineTraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params, meta = (ClampMin = 0.0f, UIMin = 0.0f, ClampMax = 1.0f, UIMax = 1.0f))
		FVector BoundingBoxLerp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bOnlyCollidingComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
		bool bIncludeFromChildActors;

	FActorBoundsCheckParams()
		: LineTraceChannel(ECC_Visibility)
		, BoundingBoxLerp(FVector(1.0f))
		, bOnlyCollidingComponents(true)
		, bIncludeFromChildActors(false)
	{}
	
	FActorBoundsCheckParams(const ECollisionChannel InLineTraceChannel, const FVector& InBoundingBoxLerp,
		const bool bInOnlyCollidingComponents, const bool bInIncludeFromChildActors)
		: LineTraceChannel(InLineTraceChannel)
		, BoundingBoxLerp(InBoundingBoxLerp)
		, bOnlyCollidingComponents(bInOnlyCollidingComponents)
		, bIncludeFromChildActors(bInIncludeFromChildActors)
	{}
	
	FActorBoundsCheckParams(const float InBoundingBoxLerp)
		: LineTraceChannel(ECC_Visibility)
		, BoundingBoxLerp(FVector(InBoundingBoxLerp))
		, bOnlyCollidingComponents(true)
		, bIncludeFromChildActors(false)
	{}
	
	TArray<FVector> ProcessVertices(const TArray<FVector>& InVertices, const FVector& Origin) const;
};

UCLASS(DisplayName = "Toro Utilities")
class TOROCORE_API UToroGeneralUtils final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* Tries to get the active Game World. Context is optional but potentially more reliable with it. */
	UFUNCTION(BlueprintPure, Category = World, meta = (AdvancedDisplay = "Context", DefaultToSelf = "Context"))
		static UWorld* GetPlayWorld(const UObject* Context);

	/* Gets the number of shaders currently compiling for a Compiling Shaders screen. */
	UFUNCTION(BlueprintPure, Category = World)
		static int32 GetNumCompilingShaders();

	/** Gets the vertices/corner locations of the actor's bounding box.
	* @param Target - Actor to use.
	* @param bOnlyCollidingComponents - If enabled, only components that can collide will be used in calculation.
	* @param bIncludeFromChildActors - Whether child actors should be included in the calculation.
	* @param Origin - Origin of the actor.
	* @param BoxExtent - Bounding box extent of the actor.
	* @return Locations of the 8 (or less) vertices of the bounding box.
	*/
	UFUNCTION(BlueprintPure, Category = Actor, meta = (DefaultToSelf = "Target"))
	static TArray<FVector> GetBoundingBoxVertices(const AActor* Target, const bool bOnlyCollidingComponents,
		const bool bIncludeFromChildActors, FVector& Origin, FVector& BoxExtent);

	/* Force a garbage collection to take place to hopefully reduce memory usage */
	UFUNCTION(BlueprintCallable, Category = Loading)
		static void ForceGarbageCollection();

	/* Get the number of currently loading async packages */
	UFUNCTION(BlueprintCallable, Category = Loading, DisplayName = "Get Num Async Packages")
		static int32 GetNumAsyncPkgs();

	/* Get the loading progress of an async package, otherwise -1 */
	UFUNCTION(BlueprintCallable, Category = Loading, DisplayName = "Get Async Load Percentage")
		static float GetAsyncPkgLoadPercent(const FString Package);

	/* Calls an event by name in the Target object. */
	UFUNCTION(BlueprintCallable, Category = Object, meta = (DefaultToSelf = "Target"))
		static void CallLocalEvent(UObject* Target, const FName EventName);
};
