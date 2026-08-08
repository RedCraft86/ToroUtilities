// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

/**
 * A wrapper for an array of pairs to mimic a Map with a fixed insertion order. Does not allow duplicates like a TMap.
 * @note Does not have the same optimizations as a TMap and operations will be O(n) as it uses an array internally.
 * @tparam K Type of key to use. (Requires equality operator)
 * @tparam V Type of value to use.
 */
template<typename K, typename V>
class TArrayMap final
{
	TArray<TPair<K, V>> Values;

	int32 IndexOf(const K& Key) const
	{
		for (int32 i = 0; i < Values.Num(); i++)
		{
			if (Values[i].Key == Key)
			{
				return i;
			}
		}

		return INDEX_NONE;
	}

public:

	TArrayMap()
		: Values()
	{}

	/**
	 * @return Array of Key-Value pairs internally used by this array map.
	 */
	TArray<TPair<K, V>>& GetValues()
	{
		return Values;
	}

	/**
	 * @return Array of Key-Value pairs internally used by this array map.
	 */
	const TArray<TPair<K, V>>& GetValues() const
	{
		return Values;
	}

	/**
	 * @return The number of entries in this array map.
	 */
	int32 Num() const
	{
		return Values.Num();
	}

	/**
	 * @return True if there are no entries in this array map
	 */
	bool IsEmpty() const
	{
		return Values.IsEmpty();
	}

	/**
	 * Checks to see if a value exist in the array map.
	 * @param Key The key to search.
	 * @return True if it exists.
	 */
	V* Contains(const K& Key)
	{
		return IndexOf(Key) != INDEX_NONE;
	}

	/**
	 * Finds an entry in the array map.
	 * @param Key The key of the value to find.
	 * @return A pointer to the value if found, null otherwise.
	 */
	V* Find(const K& Key)
	{
		const int32 Idx = IndexOf(Key);
		return (Idx == INDEX_NONE) ? nullptr : &Values[Idx].Value;
	}

	/**
	 * Finds an entry in the array map.
	 * @param Key The key of the value to find.
	 * @return A pointer to the value if found, null otherwise.
	 */
	const V* Find(const K& Key) const
	{
		const int32 Idx = IndexOf(Key);
		return (Idx == INDEX_NONE) ? nullptr : &Values[Idx].Value;
	}

	/**
	 * Add or replace an entry in the array map.
	 * @param Key The key of the value to add/overwrite.
	 * @param Value The value to add/overwrite.
	 * @param bMoveToEnd If overwriting an existing value, whether to move it to the end of the array.
	 * @return Reference to the value associated with the key.
	 */
	V& Add(const K& Key, const V& Value, const bool bMoveToEnd = false)
	{
		int32 Idx = IndexOf(Key);
		if (Idx == INDEX_NONE)
		{
			Idx = Values.Add({Key, Value});
		}
		else if (bMoveToEnd)
		{
			Values.RemoveAt(Idx);
			Idx = Values.Add({Key, Value});
		}
		else
		{
			Values[Idx].Value = Value;
		}

		return Values[Idx].Value;
	}

	/**
	 * Remove an entry from array map.
	 * @param Key The key of the value to remove.
	 * @return True if the entry was removed.
	 */
	bool Remove(const K& Key)
	{
		const int32 Idx = IndexOf(Key);
		if (Idx != INDEX_NONE)
		{
			Values.RemoveAt(Idx);
			return true;
		}

		return false;
	}

	/**
	 * Appends values from another array map to this array map.
	 * If a key exist in both array maps, the one from the other array map wins.
	 * @param Other The array map to append.
	 * @param bMoveToEnd If overwriting an existing value, whether to move it to the end of the array.
	 */
	void Append(const TArrayMap<K, V>& Other, const bool bMoveToEnd = false)
	{
		Values.Reserve(Values.Num() + Other.Values.Num());
		for (const TPair<K, V>& Value : Other.Values)
		{
			Add(Value.Key, Value.Value, bMoveToEnd);
		}
	}

	/**
	 * Removes all entries from the array map.
	 * @param Slack Expected usage after emptying.
	 */
	void Empty(const int32 Slack = 0)
	{
		Values.Empty(Slack);
	}

	/**
	 * Reserves amount of slots in the array map for better insertions.
	 * @param Count Amount to reserve. (Total; if expanding, must be Current + Additional)
	 */
	void Reserve(const int32 Count)
	{
		Values.Reserve(Count);
	}

	/**
	 * Find or add an entry in the array map.
	 * @param Key The key of the value to find/add.
	 * @return Reference to the value associated with the key.
	 */
	V& FindOrAdd(const K& Key)
	{
		if (V* Found = Find(Key))
		{
			return *Found;
		}

		return Add(Key, V());
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, TArrayMap<K, V>& ArrayMap)
	{
		return Ar << ArrayMap.Values;
	}

	FORCEINLINE friend void operator<<(FStructuredArchive::FSlot Slot, TArrayMap<K, V>& ArrayMap)
	{
		Slot << ArrayMap.Values;
	}
};