// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

/**
 * Resolves a world from an object context or from a stored fallback world.
 * The stored fallback is a weak reference and does not keep the world alive.
 */
class TORORUNTIME_API FWorldGetter final
{
	static inline TWeakObjectPtr<UWorld> CachedWorld = nullptr;

public:

	/**
	 * Resolves Context first, then uses the cached world or an engine fallback.
	 * Call Reset when an explicitly stored world should no longer be used.
	 */
	[[nodiscard]] static UWorld* Get(const UObject* Context = nullptr)
	{
		if (Context && GEngine)
		{
			CachedWorld = GEngine->GetWorldFromContextObject(Context, EGetWorldErrorMode::LogAndReturnNull);
		}

		if (CachedWorld.IsValid())
		{
			return CachedWorld.Get();
		}

		if (GEngine)
		{
			CachedWorld = GEngine->GetCurrentPlayWorld();
		}

		if (!CachedWorld.IsValid())
		{
			CachedWorld = GWorld.GetReference();
		}

		return CachedWorld.Get();
	}

	/**
	 * Stores a non-null world as the fallback for calls without a resolvable context.
	 */
	static void SetWorld(UWorld* InWorld)
	{
		if (InWorld)
		{
			CachedWorld = InWorld;
		}
	}

	/**
	 * Clears the stored fallback world.
	 */
	static void Reset()
	{
		CachedWorld.Reset();
	}
};