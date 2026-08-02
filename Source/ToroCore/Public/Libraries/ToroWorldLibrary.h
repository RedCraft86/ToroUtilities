// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "UE5Coro.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroWorldLibrary.generated.h"

/**
 * State of a streaming level.
 */
UENUM(BlueprintType)
enum class EToroLevelStreamState : uint8
{
	// Streaming level is completely unloaded.
	Unloaded,

	// Streaming level is loaded but not made visible.
	Loaded,

	// Streaming level is loaded and visible.
	Visible
};

/**
 * A utility library providing functions for the game world.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroWorldLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Attempts to find a valid UWorld pointer from <c>FWorldGetter</c>.
	 * @param Context The object to search the world from. (Can be optional if cache is set)
	 * @return A pointer to the current possible UWorld, or nullptr if unable to find.
	 */
	UFUNCTION(BlueprintPure, Category = World, meta = (AdvancedDisplay = "Context", DefaultToSelf = "Context"))
		static UWorld* GetPossibleWorld(const UObject* Context);

	/**
	 * Calls OpenLevel on the current level, effectively restarting it.
	 * @param ContextObject The object used to help find the active world to reload.
	 * @param bAbsolute If true, options are reset; if false, options are carried over from current level
	 * @param Options A string of options to use for the travel URL.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject, AdvancedDisplay = "bAbsolute, Options"))
		static void ReloadLevel(const UObject* ContextObject, const bool bAbsolute = true, const FString& Options = FString());

	/**
	 * Triggers a Remote Event defined in the Level Blueprint.
	 * @param ContextObject The object used to help find the active world to call events on.
	 * @param EventName The name of the Custom Event in the Level Blueprint.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject))
		static void CallRemoteEvent(const UObject* ContextObject, const FName EventName);

	/**
	 * Gets the current state (Unloaded, Loaded, Visible) of a streaming level in the world.
	 * @param ContextObject Object used to find the world where the streaming levels are at.
	 * @param Level Level to check the streaming state.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject))
		static EToroLevelStreamState GetLevelStreamState(const UObject* ContextObject, const TSoftObjectPtr<UWorld>& Level);

	/**
	 * Changes the current state (Unloaded, Loaded, Visible) of a streaming level in the world.
	 * @param ContextObject Object used to find the world where the streaming levels are at.
	 * @param StreamedLevel If found, the level streaming object of the specified level.
	 * @param Level Level to apply the streaming state to.
	 * @param State Streaming state to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject))
		static FVoidCoroutine SetLevelStreamState(FLatentActionInfo LatentInfo, const UObject* ContextObject,
			ULevelStreaming*& StreamedLevel, const TSoftObjectPtr<UWorld>& Level, const EToroLevelStreamState State);
};
