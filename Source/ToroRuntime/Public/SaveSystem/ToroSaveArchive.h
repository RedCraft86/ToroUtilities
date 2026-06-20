// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "UObject/SoftObjectPath.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

/**
 * A specialized proxy archive for SaveGame operations.
 * 
 * Features:
 * - Automatically filters for properties marked with the 'SaveGame' UPROPERTY flag.
 * - Disables Delta Serialization (ArNoDelta = true) to ensure full property snapshots.
 * - Intercepts UObject pointers and serializes them as string-based FSoftObjectPaths.
 * - Handles automatic loading/resolving of SoftObjectPaths during deserialization.
 */
template<bool bIsLoading>
struct TORORUNTIME_API FToroSaveGameProxyArchive : FObjectAndNameAsStringProxyArchive
{
	FToroSaveGameProxyArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		// Only serialize properties marked with the 'SaveGame' UPROPERTY flag.
		ArIsSaveGame = true;

		// Do not check against CDO for changes; save all properties as they are.
		ArNoDelta = true;
	}

	virtual FArchive& operator<<(FSoftObjectPath& Value) override
	{
		Value.SerializePath(*this);
		return *this;
	}

	virtual FArchive& operator<<(FSoftObjectPtr& Value) override
	{
		FSoftObjectPath Path;

		if (!bIsLoading)
		{
			Path = Value.ToSoftObjectPath();
		}

		*this << Path;

		if (bIsLoading)
		{
			Value = FSoftObjectPtr(Path);
		}

		return *this;
	}

	virtual FArchive& operator<<(FWeakObjectPtr& Obj) override
	{
		return SerializeObject(Obj);
	}

	virtual FArchive& operator<<(FObjectPtr& Obj) override
	{
		return SerializeObject(Obj);
	}

	virtual FArchive& operator<<(UObject*& Obj) override
	{
		return SerializeObject(Obj);
	}

private:

	static FSoftObjectPath ToSoftObjectPath(const FWeakObjectPtr& Value)
	{
		return FSoftObjectPath(Value.Get());
	}

	template<typename ObjectType>
	static FSoftObjectPath ToSoftObjectPath(const ObjectType& Value)
	{
		return FSoftObjectPath(Value);
	}

	template<typename ObjectType>
	FArchive& SerializeObject(ObjectType& Value)
	{
		FSoftObjectPath Path;

		if (!bIsLoading)
		{
			Path = ToSoftObjectPath(Value);
		}

		*this << Path;

		if (bIsLoading)
		{
			UObject* Object = Path.ResolveObject();
			Value = Object;

			if (!IsValid(Object) && !Path.IsNull())
			{
				Value = Path.TryLoad();
			}
		}

		return *this;
	}
};
