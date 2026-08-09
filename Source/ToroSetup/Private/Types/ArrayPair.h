// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

template<typename K, typename V>
class TArrayPair final
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

	TArrayPair()
		: Values()
	{}

	const TArray<TPair<K, V>>& GetValues() const
	{
		return Values;
	}

	bool IsEmpty() const
	{
		return Values.IsEmpty();
	}

	V* Find(const K& Key)
	{
		const int32 Idx = IndexOf(Key);
		return (Idx == INDEX_NONE) ? nullptr : &Values[Idx].Value;
	}

	V& Add(const K& Key, const V& Value)
	{
		int32 Idx = IndexOf(Key);
		if (Idx == INDEX_NONE)
		{
			Idx = Values.Add({Key, Value});
		}
		else
		{
			Values.RemoveAt(Idx);
			Idx = Values.Add({Key, Value});
		}

		return Values[Idx].Value;
	}

	void Append(const TArrayPair<K, V>& Other)
	{
		Values.Reserve(Values.Num() + Other.Values.Num());
		for (const TPair<K, V>& Value : Other.Values)
		{
			Add(Value.Key, Value.Value);
		}
	}

	V& FindOrAdd(const K& Key)
	{
		if (V* Found = Find(Key))
		{
			return *Found;
		}

		return Add(Key, V());
	}
};