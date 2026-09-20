// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroRuntime.h"
#include "Traits/UObjectPtrTraits.h"

/**
 * A set that optionally validates its values and automatically removes values that become invalid.
 *
 * When constructed with a validator, newly added values are rejected if validation fails. If the value
 * type is a UObject type, cleanup is also performed automatically after garbage collection.
 *
 * @tparam T Type stored by the set.
 */
template <typename T>
class TAccumulatorSet final : public TSharedFromThis<TAccumulatorSet<T>>
{
	TSet<T> InnerSet;
	TFunction<bool(const T&)> Validate;

	TAccumulatorSet(TFunction<bool(const T&)> Validator)
		: InnerSet(), Validate(MoveTemp(Validator))
	{}

public:

	DECLARE_MULTICAST_DELEGATE(FOnChangedDelegate);
	FOnChangedDelegate OnChanged;

	TAccumulatorSet()
		: InnerSet(), Validate(nullptr)
	{}

	~TAccumulatorSet()
	{
		FCoreUObjectDelegates::GetPostGarbageCollect().RemoveAll(this);
	}

	/**
	 * Creates an accumulator set without value validation.
	 * @return A shared reference to the newly created set.
	 */
	[[nodiscard]] static TSharedRef<TAccumulatorSet<T>> Create()
	{
		return MakeShareable(new TAccumulatorSet<T>(nullptr));
	}

	/**
	 * Creates an accumulator set using the supplied value validator.
	 *
	 * For UObject value types, the set automatically removes invalid values after garbage collection.
	 *
	 * @param Validator Function used to determine whether a value is valid.
	 * @return A shared reference to the newly created set.
	 */
	[[nodiscard]] static TSharedRef<TAccumulatorSet<T>> Create(TFunction<bool(const T&)> Validator)
	{
		checkf(Validator, TEXT("Validator is null. Use the overload with no parameters if this is intentional."));

		TSharedRef<TAccumulatorSet<T>> Instance = MakeShareable(new TAccumulatorSet<T>(MoveTemp(Validator)));
		if constexpr (TIsUObjectType<T>::Value)
		{
			FCoreUObjectDelegates::GetPostGarbageCollect().AddSP(Instance, &TAccumulatorSet::Cleanup, true);
		}

		return Instance;
	}

	/**
	 * Returns the underlying set.
	 * @return Read-only reference to the stored values.
	 */
	const TSet<T>& GetInnerSet() const
	{
		return InnerSet;
	}

	/**
	 * The number of values currently stored in the set.
	 */
	int32 Num() const
	{
		return InnerSet.Num();
	}

	/**
	 * True if the set contains no values.
	 */
	bool IsEmpty() const
	{
		return InnerSet.IsEmpty();
	}

	/**
	 * Adds a value if it is not already present and passes validation.
	 * If the value is added, existing invalid values are cleaned up and OnChanged is broadcasted.
	 * @param Value Value to add.
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
	 * Removes the supplied value from the set.
	 * If a value is removed, existing invalid values are cleaned up and OnChanged is broadcasted.
	 * @param Value Value to remove.
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
	 * Invokes a predicate for each stored value until all values have been visited or the predicate returns false.
	 * @param Predicate Function called with each value. Return false to stop iteration early.
	 */
	void ForEach(const TFunction<bool(const T&)>& Predicate) const
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
	 * Removes all values that fail validation.
	 * @param bNotifyChange If true, broadcasts OnChanged when at least one value is removed.
	 *        If no validator exists, an explicit cleanup request logs a warning instead.
	 */
	void Cleanup(const bool bNotifyChange)
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

			if (bChanged && bNotifyChange)
			{
				OnChanged.Broadcast();
			}
		}
		else if (bNotifyChange)
		{
			// Internal cleanup calls do not request notification. An explicit cleanup request
			// without a validator is likely unintended, so report it rather than failing silently.
			UE_LOG(LogToroRuntime, Warning, TEXT("Called TAccumulatorSet::Cleanup without a valid validator."));
		}
	}
};
