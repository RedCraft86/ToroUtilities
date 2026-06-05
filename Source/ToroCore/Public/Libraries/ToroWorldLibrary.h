// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroWorldLibrary.generated.h"

/**
 * A specialized utility library for world-level operations and level management.
 * Provides safe methods for world context retrieval and cross-level event triggering.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroWorldLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Attempts to find a valid UWorld pointer from FWorldGetter which is a global world pointer caching class.
	 * 
	 * @param Context  (Optional) The object to search the world from.
	 * @return         A pointer to the UWorld associated with the context, or nullptr.
	 */
	UFUNCTION(BlueprintPure, Category = World, meta = (AdvancedDisplay = "Context", DefaultToSelf = "Context"))
		static UWorld* GetPossibleWorld(const UObject* Context);

	/** 
	 * Re-initializes the current level.
	 * This effectively restarts the level for all players, resetting the game state.
	 * 
	 * @param ContextObject  The object used to determine the world to reload.
	 * @param Options        Optional URL-style parameters to pass to the level on reload (e.g., "?game=MyGameMode").
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject, AdvancedDisplay = "Options"))
		static void ReloadLevel(const UObject* ContextObject, const FString& Options = TEXT(""));

	/** 
	 * Triggers a Remote Event defined in the Level Blueprint.
	 * Remote events allow for global communication between the Game Thread and level-specific logic.
	 * 
	 * @param ContextObject  World context for locating the level.
	 * @param EventName      The name of the Custom Event in the Level Blueprint marked as 'Remote'.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject))
		static void CallRemoteEvent(const UObject* ContextObject, const FName EventName);

	/**
	 * Retrieves the transform of the active camera.
	 * 
	 * In the Editor (non-PIE), this returns the transform of the active Level Viewport camera.
	 * In Game (or PIE), this returns the transform from the Player Camera Manager for the specified player index.
	 *
	 * @param ContextObject  Object used to determine the world context (needed for runtime lookup).
	 * @param PlayerIdx      The index of the player whose camera should be retrieved (Default is 0).
	 * @return               The world-space transform of the active camera. Returns FTransform::Identity if no camera is found.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject))
		static FTransform GetMainCameraTransform(const UObject* ContextObject, const int32 PlayerIdx = 0);
};
