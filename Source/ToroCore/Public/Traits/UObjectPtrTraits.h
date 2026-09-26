// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

template<typename T>
struct TIsUObjectType
{
	static constexpr bool Value = false;
};

template<typename T>
struct TIsUObjectType<T*>
{
	static constexpr bool Value = TIsDerivedFrom<T, UObject>::Value;
};

template<typename T>
struct TIsUObjectType<TObjectPtr<T>>
{
	static constexpr bool Value = true;
};

template<typename T>
struct TIsUObjectType<TWeakObjectPtr<T>>
{
	static constexpr bool Value = true;
};

template<typename T>
struct TIsUObjectType<TSoftObjectPtr<T>>
{
	static constexpr bool Value = true;
};
