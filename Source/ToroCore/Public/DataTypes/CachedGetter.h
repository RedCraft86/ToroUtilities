// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma	once

#include "Templates/Function.h"
#include "UObject/WeakObjectPtrTemplates.h"

/**
 * A utility class for lazy loading/searching and caching of UObjects.
 * Uses a TFunction passed in via the constructor to fetch the object 
 * and stores in a TWeakObjectPtr to not interfere with the Garbage Collector.
 * @tparam T a type of UObject.
 */
template<typename T>
class TCachedGetter final
{
	TCachedGetter() = delete;

	TFunction<T*()> Getter;
	TWeakObjectPtr<T> Cache;

public:

	/**
	 * Constructor requiring a valid getter function.
	 * @param GetFunction The lambda used to populate the cache.
	 */
	TCachedGetter(const TFunction<T*()>& GetFunction): Getter(GetFunction)
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value, "T must derive from UObject");
		Cache.Reset();
	}

	/** Checks if the internal cache has a valid pointer. */
	FORCEINLINE bool IsValid() const
	{
		return Cache.IsValid();
	}

	/** 
	 * Forces a call to the Getter and updates the cache, regardless of current cache state.
	 * @return The newly fetched object pointer or null if unable to get.
	 */
	T* Fetch()
	{
		if (ensureAlwaysMsgf(Getter, TEXT("TCachedGetter has no getter bound!")))
		{
			Cache = Getter();
		}

		return Cache.Get();
	}

	/** 
	 * Returns the cached object if valid; otherwise, executes the Getter to fill the cache.
	 * @return The current, newly fetched object pointer, or null if unable to get.
	 */
	T* Get()
	{
		return IsValid() ? Cache.Get() : Fetch();
	}

	/** Internally calls ::Get() */
	[[nodiscard]] T* operator->()
	{
		return Get();
	}
};