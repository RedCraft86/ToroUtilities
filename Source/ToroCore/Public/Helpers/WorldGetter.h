// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Engine/Engine.h"

/**
 * A static utility class for UWorld fetching and caching.
 * 
 * <b>Usage Example:</b>
 * <pre>
 *     UGameplayStatics::GetPlayerController(FWorldGetter::Get(OptionalContext), 0);
 * </pre>
 */
class TOROCORE_API FWorldGetter final
{
	static inline TWeakObjectPtr<UWorld> CachedWorld = nullptr;

public:

	/**
	 * If a valid World is already cached, it is returned immediately. Otherwise, it 
	 * attempts to resolve the World via the provided Context object or GEngine globals.
	 * @param Context (Optional) An object belonging to the desired world. Recommended to 
	 *        always try and provide one as it is required for the first call per world.
	 * @return A pointer to the current UWorld, or nullptr if no world could be resolved.
	 */
	[[nodiscard]] static UWorld* Get(const UObject* Context = nullptr)
	{
		if (CachedWorld.IsValid())
		{
			return CachedWorld.Get();
		}

		if (GEngine)
		{
			CachedWorld = GEngine->GetWorldFromContextObject(Context, EGetWorldErrorMode::ReturnNull);
			if (!CachedWorld.IsValid())
			{
				CachedWorld = GEngine->GetCurrentPlayWorld();
			}
		}
		else
		{
			CachedWorld = GWorld.GetReference();
		}

		return CachedWorld.Get();
	}

	/**
	 * Manually updates the internal cache with a specific World pointer.
	 * @param InWorld The world instance to cache.
	 */
	static void SetWorld(UWorld* InWorld)
	{
		if (InWorld)
		{
			CachedWorld = InWorld;
		}
	}

	/**
	 * Call to clear the weak pointer so subsequent Get() calls re-evaluate the context.
	 * @note Automatically called during map changes in <c>ToroCore.cpp</c>
	 */
	static void Reset()
	{
		CachedWorld.Reset();
	}
};