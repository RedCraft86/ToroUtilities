// Copyright (C) RedCraft86. All Rights Reserved.

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
	
	/** Checks if actor is on the player screen.
	* @param Target - Actor to check.
	* @param MaxDistance - The maximum distance before the actor is considered no longer in sight.
	* @param bOriginOnly - If enabled, the checks will only make use of the actor's origin, bounds will not be used.
	* @param bLineTrace - Draw a line trace from the camera to the target when checking if it's in player view.
	* @param TraceParams - Settings to use when testing the different vertices of the actor's bounding box.
	* @return Whether the actor is visible in screen with the given settings.
	*/
	UFUNCTION(BlueprintPure, Category = Actor, meta = (DefaultToSelf = "Target", AutoCreateRefTerm = "TraceCheckParams"))
	static bool IsActorOnScreen(const AActor* Target, const float MaxDistance = 5000.0f, const bool bOriginOnly = false,
		const bool bLineTrace = true, const FActorBoundsCheckParams& TraceParams = FActorBoundsCheckParams());

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

	/* Check if we're currently in editor or in a PIE/Standalone game. */
	UFUNCTION(BlueprintPure, Category = Misc)
		static bool IsInEditor();

	/* EDITOR ONLY - Adds a component to an actor similar to the add component functions but also refreshes component list to include this new one. */
	UFUNCTION(BlueprintCallable, Category = Editor, meta = (DefaultToSelf = "Target", DeterminesOutputType = "InClass"))
		static UActorComponent* AddActorInstanceComponent(AActor* Target, const TSubclassOf<UActorComponent> InClass);
};
