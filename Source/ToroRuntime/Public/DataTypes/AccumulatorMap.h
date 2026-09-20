// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroRuntime.h"
#include "Traits/UObjectPtrTraits.h"

/**
 * A map that optionally validates its keys and automatically removes entries whose keys become invalid.
 *
 * When constructed with a validator, newly added keys are rejected if validation fails. If the key type
 * is a UObject type, cleanup is also performed automatically after garbage collection.
 *
 * @tparam K Type used for map keys.
 * @tparam V Type used for map values.
 */
template <typename K, typename V>
class TAccumulatorMap final : public TSharedFromThis<TAccumulatorMap<K, V>>
{
	TMap<K, V> InnerMap;
	TFunction<bool(const K&)> Validate;

	TAccumulatorMap(TFunction<bool(const K&)> Validator)
		: InnerMap(), Validate(MoveTemp(Validator))
	{}

public:

	DECLARE_MULTICAST_DELEGATE(FOnChangedDelegate);
	FOnChangedDelegate OnChanged;

	TAccumulatorMap()
		: InnerMap(), Validate(nullptr)
	{}

	~TAccumulatorMap()
	{
		FCoreUObjectDelegates::GetPostGarbageCollect().RemoveAll(this);
	}

	/**
	 * Creates an accumulator map without key validation.
	 * @return A shared reference to the newly created map.
	 */
	[[nodiscard]] static TSharedRef<TAccumulatorMap<K, V>> Create()
	{
		return MakeShareable(new TAccumulatorMap<K, V>(nullptr));
	}

	/**
	 * Creates an accumulator map using the supplied key validator.
	 *
	 * For UObject key types, the map automatically removes invalid entries after garbage collection.
	 *
	 * @param Validator Function used to determine whether a key is valid.
	 * @return A shared reference to the newly created map.
	 */
	[[nodiscard]] static TSharedRef<TAccumulatorMap<K, V>> Create(TFunction<bool(const K&)> Validator)
	{
		checkf(Validator, TEXT("Validator is null. Use the overload with no parameters if this is intentional."));

		TSharedRef<TAccumulatorMap<K, V>> Instance = MakeShareable(new TAccumulatorMap<K, V>(MoveTemp(Validator)));
		if constexpr (TIsUObjectType<K>::Value)
		{
			FCoreUObjectDelegates::GetPostGarbageCollect().AddSP(Instance, &TAccumulatorMap::Cleanup, true);
		}

		return Instance;
	}

	/**
	 * Returns the underlying map.
	 * @return Read-only reference to the stored key-value pairs.
	 */
	const TMap<K, V>& GetInnerMap() const
	{
		return InnerMap;
	}

	/**
	 * The number of values currently stored in the map.
	 */
	int32 Num() const
	{
		return InnerMap.Num();
	}

	/**
	 * True if the map contains no values.
	 */
	bool IsEmpty() const
	{
		return InnerMap.IsEmpty();
	}

	/**
	 * Adds a key-value pair if the key is not already present and passes validation.
	 * If an entry is added, existing invalid entries are cleaned up and OnChanged is broadcasted.
	 * @param Key Key to add.
	 * @param Value Value associated with the key.
	 */
	void Add(const K& Key, const V& Value)
	{
		if ((Validate && !Validate(Key)) || InnerMap.Contains(Key))
		{
			return;
		}

		InnerMap.Add(Key, Value);

		Cleanup(false);
		OnChanged.Broadcast();
	}

	/**
	 * Removes the entry associated with the supplied key.
	 * If an entry is removed, existing invalid entries are cleaned up and OnChanged is broadcasted.
	 * @param Key Key whose entry should be removed.
	 */
	void Remove(const K& Key)
	{
		if (InnerMap.Remove(Key) > 0)
		{
			Cleanup(false);
			OnChanged.Broadcast();
		}
	}

	/**
	 * Invokes a predicate for each stored entry until all entries have been visited or the predicate returns false.
	 * @param Predicate Function called with each key and value. Return false to stop iteration early.
	 */
	void ForEach(const TFunction<bool(const K&, const V&)>& Predicate) const
	{
		for (const TPair<K, V>& Entry : InnerMap)
		{
			if (!Predicate(Entry.Key, Entry.Value))
			{
				break;
			}
		}
	}

	/**
	 * Removes all entries whose keys fail validation.
	 * @param bNotifyChange If true, broadcasts OnChanged when at least one entry is removed.
	 *        If no validator exists, an explicit cleanup request logs a warning instead.
	 */
	void Cleanup(const bool bNotifyChange)
	{
		if (Validate)
		{
			bool bChanged = false;
			for (auto It = InnerMap.CreateIterator(); It; ++It)
			{
				if (!Validate(It.Key()))
				{
					It.RemoveCurrent();
					bChanged = true;
				}
			}

			if (bChanged && bNotifyChange)
			{
				OnChanged.Broadcast();
			}
		}
		else if (bNotifyChange)
		{
			// Internal cleanup calls do not request notification. An explicit cleanup request
			// without a validator is likely unintended, so report it rather than failing silently.
			UE_LOG(LogToroRuntime, Warning, TEXT("Called TAccumulatorMap::Cleanup without a valid validator."));
		}
	}
};
