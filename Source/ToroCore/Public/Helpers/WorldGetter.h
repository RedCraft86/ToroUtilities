// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/Engine.h"

/**
 * A static utility class for centralized UWorld retrieval and caching.
 *
 * This class provides a convenient way to access the current World pointer 
 * while minimizing expensive lookups by caching a weak reference.
 */
class TOROCORE_API FWorldGetter final
{
	static inline TWeakObjectPtr<UWorld> CachedWorld = nullptr;

public:

	/**
	 * If a valid World is already cached, it is returned immediately. Otherwise, it 
	 * attempts to resolve the World via the provided Context object or GEngine globals.
	 *
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
	 * Call during map changes or module shutdowns to ensure 
	 * subsequent calls to Get() re-evaluate the context.
	 * 
	 * This function is already called for the above situations in ToroCore.cpp
	 */
	static void Reset()
	{
		CachedWorld.Reset();
	}
};
