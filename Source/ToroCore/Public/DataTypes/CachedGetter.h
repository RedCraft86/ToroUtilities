// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ToroCore.h"

/**
 * A utility wrapper for lazy-loading and caching UObject pointers.
 * Uses a TFunction to fetch the object and stores it in a TWeakObjectPtr to ensure 
 * the cache doesn't prevent Garbage Collection and handles stale pointers gracefully.
 * 
 * @tparam T The type of object to be cached. Must derive from UObject.
 */
template<typename T>
class TCachedGetter final
{
	// Unused constructor
	TCachedGetter() = delete;

	TFunction<T*()> Getter;
	TWeakObjectPtr<T> CachedPtr;

public:

	/**
	 * Constructor requiring a valid getter function.
	 * @param InGetter The function/lambda used to populate the cache.
	 */
	TCachedGetter(const TFunction<T*()>& InGetter): Getter(InGetter)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value, "T must derive from UObject");
		CachedPtr.Reset();
	}

	/** 
	 * Internally calls ::Get()
	 */
	[[nodiscard]] FORCEINLINE T* operator->()
	{
		return Get();
	}

	/** 
	 * Returns the cached object if valid; otherwise, executes the Getter to refresh the cache.
	 * @return The current or newly fetched object pointer.
	 */
	[[nodiscard]] T* Get()
	{
		return CachedPtr.IsValid() ? CachedPtr.Get() : Fetch();
	}

	/** 
	 * Forces a call to the Getter and updates the cache, regardless of current cache state.
	 * @return The freshly fetched object pointer.
	 */
	[[nodiscard]] T* Fetch()
	{
		if (ensureAlwaysMsgf(Getter, TEXT("TCachedGetter has no getter bound!")))
		{
			CachedPtr = Getter();
		}
		return CachedPtr.Get();
	}
};