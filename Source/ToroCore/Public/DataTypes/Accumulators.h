// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToroCore.h"
#include "Helpers/UObjectPtrTraits.h"

/**
 * A TSet wrapper focused for accumulation of things.
 * When something is added or removed, fires a delegate others can listen to.
 * If type is a UObject, this will bind to garbage collection events for automatic cleanup.
 * @tparam T Type of thing that will be accumulated. (Requires GetTypeHash impl)
 */
template<typename T>
class TAccumulatorSet final : public TSharedFromThis<TAccumulatorSet<T>>
{
	TSet<T> InnerSet;
	TFunction<bool(const T&)> Validate;

	/**
	 * Internal constructor that takes a validator.
	 * @param Validator A valid validator or null if one is not needed.
	 */
	TAccumulatorSet(TFunction<bool(const T&)> Validator)
		: InnerSet(), Validate(MoveTemp(Validator))
	{}

public:

	/**
	 * Create an accumulator set without a validator.
	 * @return A new accumulator set.
	 */
	static TSharedRef<TAccumulatorSet<T>> Create()
	{
		return MakeShareable(new TAccumulatorSet<T>(nullptr));
	}

	/**
	 * Create an accumulator set with a validator that validate entries.
	 * Binds <c>::Cleanup</c> to <c>FCoreUObjectDelegates::GetPostGarbageCollect()</c> if type is a UObject.
	 * @return A new accumulator set.
	 */
	static TSharedRef<TAccumulatorSet<T>> Create(TFunction<bool(const T&)> Validator)
	{
		checkf(Validator, TEXT("Validator is null. Use the overload with no parameters if this is intentional."))

		TSharedRef<TAccumulatorSet<T>> Instance = MakeShareable(new TAccumulatorSet<T>(MoveTemp(Validator)));
		if constexpr (TIsUObjectType<T>::Value)
		{
			FCoreUObjectDelegates::GetPostGarbageCollect().AddSP(Instance, &TAccumulatorSet::Cleanup, true);
		}

		return Instance;
	}

	/**
	 * Default constructor used by TSharedRef internals.
	 * @note SHOULD NOT BE CALLED MANUALLY!!!
	 */
	TAccumulatorSet()
		: InnerSet(), Validate(nullptr)
	{}

	/**
	 * Destructor that unbinds from <c>FCoreUObjectDelegates::GetPostGarbageCollect()</c> if bound.
	 */
	~TAccumulatorSet()
	{
		FCoreUObjectDelegates::GetPostGarbageCollect().RemoveAll(this);
	}

	/**
	 * @return The set that is internally used by this accumulator.
	 */
	const TSet<T>& GetInnerMap()
	{
		return InnerSet;
	}

	/**
	 * @return The number of entries in this accumulator.
	 */
	int32 Num()
	{
		return InnerSet.Num();
	}

	/**
	 * @return True if there are no entries in this accumulator
	 */
	bool IsEmpty()
	{
		return InnerSet.IsEmpty();
	}

	/**
	 * Adds a value to the accumulator and broadcast the changed variable.
	 * Also calls <c>::Cleanup()</c> before the broadcast.
	 * @note Value must pass the provided Validator to be added.
	 * @param Value The value to add.
	 */
	void Add(const T& Value)
	{
		if ((Validate && !Validate(Value)) || InnerSet.Contains(Value))
		{
			return;
		}

		InnerSet.Add(Value);

		Cleanup(false);
		OnChanged.Broadcast();
	}

	/**
	 * Removes a value from the accumulator and broadcast the changed variable.
	 * Also calls <c>::Cleanup()</c> before the broadcast.
	 * @param Value The value to remove.
	 */
	void Remove(const T& Value)
	{
		if (InnerSet.Remove(Value) > 0)
		{
			Cleanup(false);
			OnChanged.Broadcast();
		}
	}

	/**
	 * Iterate through all entries in the accumulator.
	 * @param Predicate A function called for each element. Return false to terminate the loop early.
	 */
	void ForEach(const TFunction<bool(const T&)>& Predicate)
	{
		for (const T& Value : InnerSet)
		{
			if (!Predicate(Value))
			{
				break;
			}
		}
	}

	/**
	 * Calls the validator on each of the entries and remove ones that are not valid. Does nothing without a validator.
	 * @param bNotifyIfChanged If true, <c>OnChanged</c> will broadcast if anything was removed.
	 */
	void Cleanup(const bool bNotifyIfChanged)
	{
		if (Validate)
		{
			bool bChanged = false;
			for (auto It = InnerSet.CreateIterator(); It; ++It)
			{
				if (!Validate(*It))
				{
					It.RemoveCurrent();
					bChanged = true;
				}
			}

			if (bChanged && bNotifyIfChanged)
			{
				OnChanged.Broadcast();
			}
		}
		else if (bNotifyIfChanged)
		{
			UE_LOG(LogToroCore, Warning, TEXT("Called TAccumulatorSet::Cleanup with Notify but validator is null."))
		}
	}

	DECLARE_MULTICAST_DELEGATE(FOnChangedDelegate);
	FOnChangedDelegate OnChanged;
};

/**
 * A TMap wrapper focused for accumulation of things.
 * When something is added/overwritten or removed, fires a delegate others can listen to.
 * If key type is a UObject, this will bind to garbage collection events for automatic cleanup.
 * @tparam K A type of key that will be used to give accumulated values an identity. (Requires GetTypeHash impl)
 * @tparam V A type of value that will be accumulated.
 */
template<typename K, typename V>
class TAccumulatorMap final : public TSharedFromThis<TAccumulatorMap<K, V>>
{
	TMap<K, V> InnerMap;
	TFunction<bool(const K&)> Validate;

	TAccumulatorMap(TFunction<bool(const K&)> Validator)
		: InnerMap(), Validate(MoveTemp(Validator))
	{}

public:

	/**
	 * Create an accumulator map without a validator.
	 * @return A new accumulator map.
	 */
	static TSharedRef<TAccumulatorMap<K, V>> Create()
	{
		return MakeShareable(new TAccumulatorMap<K, V>(nullptr));
	}

	/**
	 * Create an accumulator map with a validator that validate keys.
	 * Binds <c>::Cleanup</c> to <c>FCoreUObjectDelegates::GetPostGarbageCollect()</c> if key type is a UObject.
	 * @return A new accumulator map.
	 */
	static TSharedRef<TAccumulatorMap<K, V>> Create(TFunction<bool(const K&)> Validator)
	{
		checkf(Validator, TEXT("Validator is null. Use the overload with no parameters if this is intentional."))

		TSharedRef<TAccumulatorMap<K, V>> Instance = MakeShareable(new TAccumulatorMap<K, V>(MoveTemp(Validator)));
		if constexpr (TIsUObjectType<K>::Value)
		{
			FCoreUObjectDelegates::GetPostGarbageCollect().AddSP(Instance, &TAccumulatorMap::Cleanup, true);
		}

		return Instance;
	}

	/**
	 * Default constructor used by TSharedRef internals.
	 * @note SHOULD NOT BE CALLED MANUALLY!!!
	 */
	TAccumulatorMap()
		: InnerMap(), Validate(nullptr)
	{}

	/**
	 * Destructor that unbinds from <c>FCoreUObjectDelegates::GetPostGarbageCollect()</c> if bound.
	 */
	~TAccumulatorMap()
	{
		FCoreUObjectDelegates::GetPostGarbageCollect().RemoveAll(this);
	}

	/**
	 * @return The map that is internally used by this accumulator.
	 */
	const TMap<K, V>& GetInnerMap()
	{
		return InnerMap;
	}

	/**
	 * @return The number of entries in this accumulator.
	 */
	int32 Num()
	{
		return InnerMap.Num();
	}

	/**
	 * @return True if there are no entries in this accumulator
	 */
	bool IsEmpty()
	{
		return InnerMap.IsEmpty();
	}

	/**
	 * Adds a value to the accumulator and broadcast the changed variable.
	 * Also calls <c>::Cleanup()</c> before the broadcast.
	 * @note Key must pass the provided Validator to be added.
	 * @param Key The key of the value to add/overwrite.
	 * @param Value The value to add/overwrite.
	 * @param bSkipDuplicate If true, value will not overwrite if the same key already exist.
	 */
	void Add(const K& Key, const V& Value, const bool bSkipDuplicate = false)
	{
		if ((Validate && !Validate(Key)) || (bSkipDuplicate && InnerMap.Contains(Key)))
		{
			return;
		}

		InnerMap.Add(Key, Value);

		Cleanup(false);
		OnChanged.Broadcast();
	}

	/**
	 * Removes a value from the accumulator and broadcast the changed variable.
	 * Also calls <c>::Cleanup()</c> before the broadcast.
	 * @param Key The key of the value to remove.
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
	 * Iterate through all entries in the accumulator.
	 * @param Predicate A function called for each key value pair. Return false to terminate the loop early.
	 */
	void ForEach(const TFunction<bool(const TPair<K, V>&)>& Predicate)
	{
		for (const TPair<K, V>& Pair : InnerMap)
		{
			if (!Predicate(Pair))
			{
				break;
			}
		}
	}

	/**
	 * Calls the validator on each of the keys and remove entries that are not valid. Does nothing without a validator.
	 * @param bNotifyIfChanged If true, <c>OnChanged</c> will broadcast if anything was removed.
	 */
	void Cleanup(const bool bNotifyIfChanged)
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

			if (bChanged && bNotifyIfChanged)
			{
				OnChanged.Broadcast();
			}
		}
		else if (bNotifyIfChanged)
		{
			UE_LOG(LogToroCore, Warning, TEXT("Called TAccumulatorMap::Cleanup with Notify but validator is null."))
		}
	}

	DECLARE_MULTICAST_DELEGATE(FOnChangedDelegate);
	FOnChangedDelegate OnChanged;
};