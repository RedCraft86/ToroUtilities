// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

/**
 * Lazily retrieves and weakly caches a UObject instance.
 *
 * The supplied getter is invoked when no valid object is currently cached.
 * Because the cache uses a weak object pointer, it does not prevent the
 * retrieved object from being garbage collected.
 *
 * @tparam T UObject-derived type returned by the getter.
 */
template<typename T>
class TCachedGetter final
{
	TWeakObjectPtr<T> Cache;
	TFunction<T*()> Getter;

	TCachedGetter() = delete;

public:

	/**
	 * Creates a cached getter from the function used to retrieve the object.
	 * @param GetterFunc Function invoked whenever the cache must be refreshed.
	 */
	TCachedGetter(TFunction<T*()> GetterFunc)
		: Getter(MoveTemp(GetterFunc))
	{
		static_assert(TIsDerivedFrom<T, UObject>::Value, "T must derive from a UObject");
		Cache.Reset();
	}

	/**
	 * Returns the cached object, fetching it first when necessary.
	 * @return The cached object, or nullptr if the getter does not provide one.
	 */
	T* operator->()
	{
		return Get();
	}

	/**
	 * Checks whether the cache currently refers to a valid UObject.
	 * @return true when the cached object is valid; otherwise, false.
	 */
	[[nodiscard]] FORCEINLINE bool IsValid() const
	{
		return Cache.IsValid();
	}

	/**
	 * Invokes the getter and replaces the currently cached object.
	 * @return The newly cached object, or nullptr if the getter is unbound or
	 *         does not provide a valid object.
	 */
	T* Fetch()
	{
		if (ensureMsgf(Getter, TEXT("TCachedGetter has no getter bound.")))
		{
			Cache = Getter();
		}

		return Cache.Get();
	}

	/**
	 * Returns the cached object, invoking the getter when the cache is invalid.
	 * @return The cached or newly fetched object, or nullptr if none is available.
	 */
	T* Get()
	{
		return IsValid() ? Cache.Get() : Fetch();
	}
};