// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ToroCore.h"
#include "Helpers/UObjectTraits.h"

template<typename T>
class TAccumulatorSet final : public TSharedFromThis<TAccumulatorSet<T>>
{
	TSet<T> InnerSet;
	TFunction<bool(const T&)> Validate;

	TAccumulatorSet(TFunction<bool(const T&)> Validator)
		: InnerSet(), Validate(MoveTemp(Validator))
	{}

public:

	static TSharedRef<TAccumulatorSet<T>> Create()
	{
		return MakeShareable(new TAccumulatorSet<T>(nullptr));
	}

	static TSharedRef<TAccumulatorSet<T>> Create(TFunction<bool(const T&)> Validator)
	{
		TSharedRef<TAccumulatorSet<T>> Instance = MakeShareable(new TAccumulatorSet<T>(MoveTemp(Validator)));
		if constexpr (TIsUObjectType<T>::Value)
		{
			FCoreUObjectDelegates::GetPostGarbageCollect().AddSP(Instance, &TAccumulatorSet::Cleanup, true);
		}

		return Instance;
	}

	TAccumulatorSet()
		: InnerSet(), Validate(nullptr)
	{}

	~TAccumulatorSet()
	{
		FCoreUObjectDelegates::GetPostGarbageCollect().RemoveAll(this);
	}

	const TSet<T>& GetInnerMap()
	{
		return InnerSet;
	}

	int32 Num()
	{
		return InnerSet.Num();
	}

	bool IsEmpty()
	{
		return InnerSet.IsEmpty();
	}

	void Add(const T& Value)
	{
		if (InnerSet.Contains(Value))
		{
			return;
		}

		InnerSet.Add(Value);

		Cleanup(false);
		OnChanged.Broadcast();
	}

	void Remove(const T& Key)
	{
		if (InnerSet.Remove(Key) > 0)
		{
			Cleanup(false);
			OnChanged.Broadcast();
		}
	}

	void ForEach(const TFunction<bool(const T&)>& Iteration)
	{
		for (const T& Value : InnerSet)
		{
			if (!Iteration(Value))
			{
				return;
			}
		}
	}

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

template<typename K, typename V>
class TAccumulatorMap final : public TSharedFromThis<TAccumulatorMap<K, V>>
{
	TMap<K, V> InnerMap;
	TFunction<bool(const K&)> Validate;

	TAccumulatorMap(TFunction<bool(const K&)> Validator)
		: InnerMap(), Validate(MoveTemp(Validator))
	{}

public:

	static TSharedRef<TAccumulatorMap<K, V>> Create()
	{
		return MakeShareable(new TAccumulatorMap<K, V>(nullptr));
	}

	static TSharedRef<TAccumulatorMap<K, V>> Create(TFunction<bool(const K&)> Validator)
	{
		TSharedRef<TAccumulatorMap<K, V>> Instance = MakeShareable(new TAccumulatorMap<K, V>(MoveTemp(Validator)));
		if constexpr (TIsUObjectType<K>::Value)
		{
			FCoreUObjectDelegates::GetPostGarbageCollect().AddSP(Instance, &TAccumulatorMap::Cleanup, true);
		}

		return Instance;
	}

	TAccumulatorMap()
		: InnerMap(), Validate(nullptr)
	{}

	~TAccumulatorMap()
	{
		FCoreUObjectDelegates::GetPostGarbageCollect().RemoveAll(this);
	}

	const TMap<K, V>& GetInnerMap()
	{
		return InnerMap;
	}

	int32 Num()
	{
		return InnerMap.Num();
	}

	bool IsEmpty()
	{
		return InnerMap.IsEmpty();
	}

	void Add(const K& Key, const V& Value, const bool bSkipDuplicate = false)
	{
		if (bSkipDuplicate && InnerMap.Contains(Key))
		{
			return;
		}

		InnerMap.Add(Key, Value);

		Cleanup(false);
		OnChanged.Broadcast();
	}

	void Remove(const K& Key)
	{
		if (InnerMap.Remove(Key) > 0)
		{
			Cleanup(false);
			OnChanged.Broadcast();
		}
	}

	void ForEach(const TFunction<bool(const TPair<K, V>&)>& Iteration)
	{
		for (const TPair<K, V>& Pair : InnerMap)
		{
			if (!Iteration(Pair))
			{
				return;
			}
		}
	}

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