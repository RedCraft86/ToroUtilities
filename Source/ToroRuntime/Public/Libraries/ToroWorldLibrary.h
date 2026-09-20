// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "UE5Coro.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroWorldLibrary.generated.h"

/**
 * Describes whether a streamed level is unloaded, loaded but hidden, or visible.
 */
UENUM(BlueprintType)
enum class EToroLevelStreamState : uint8
{
	Unloaded,
	Loaded,
	Visible
};

/**
 * Provides world, level, and remote event helpers.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroWorldLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Finds a world associated with an object.
	 * @param Context Object used to resolve the world.
	 */
	UFUNCTION(BlueprintPure, Category = World, meta = (DefaultToSelf = Context, AdvancedDisplay = Context))
		static UWorld* GetPossibleWorld(const UObject* Context);

	/**
	 * Reopens the current level.
	 * @param ContextObject Object used to find the world.
	 * @param bAbsolute Whether to discard existing URL options.
	 * @param Options Additional travel options.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject, AdvancedDisplay = "bAbsolute, Options"))
		static void ReloadLevel(const UObject* ContextObject, const bool bAbsolute = true, const FString& Options = FString());

	/**
	 * Invokes a named remote event on the current level script actor.
	 * @param ContextObject Object used to find the world.
	 * @param EventName Remote event name.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject))
		static void CallRemoteEvent(const UObject* ContextObject, const FName EventName);

	/**
	 * Gets the requested load and visibility state of a streaming level.
	 * @param ContextObject Object used to find the world.
	 * @param Level Streaming level asset.
	 * @return Unloaded when the streaming level is missing or not requested to load.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (WorldContext = ContextObject))
		static EToroLevelStreamState GetLevelStreamState(const UObject* ContextObject, const TSoftObjectPtr<UWorld>& Level);

	/**
	 * Loads, shows, or unloads a streaming level and waits for completion.
	 * @param LatentInfo Blueprint latent action information.
	 * @param ContextObject Object used to find the world.
	 * @param StreamedLevel Receives the streaming level object, or nullptr if missing.
	 * @param Level Streaming level asset.
	 * @param State Requested streaming state.
	 */
	UFUNCTION(BlueprintCallable, Category = World, meta = (Latent, LatentInfo = LatentInfo, WorldContext = ContextObject))
		static FVoidCoroutine SetLevelStreamState(FLatentActionInfo LatentInfo, const UObject* ContextObject,
			ULevelStreaming*& StreamedLevel, const TSoftObjectPtr<UWorld>& Level, const EToroLevelStreamState State);
};
