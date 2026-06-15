// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ToroRuntime.h"
#include "ISavableObject.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

/**
 * An intermediary proxy archive used to translate UObject and FName references 
 * into string-based paths during serialization. This prevents "stale pointer" 
 * issues when reloading data in a different session.
 */
struct TORORUNTIME_API FToroSaveGameProxyArchive : FObjectAndNameAsStringProxyArchive
{
	FToroSaveGameProxyArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		// Only serialize properties marked with the 'SaveGame' UPROPERTY flag.
		ArIsSaveGame = true;

		// Do not use delta serialization; ensure the full state is captured.
		ArNoDelta = true;
	}
};

/**
 * A memory writer that handles the high-level logic for serializing 
 * UObjects and various smart pointers into a byte array.
 */
class TORORUNTIME_API FToroSaveGameArchive : public FMemoryWriter
{
public:

	FToroSaveGameArchive(TArray<uint8>& OutBytes)
		: FMemoryWriter(OutBytes, true)
	{
		Seek(0);
	}

	virtual FString GetArchiveName() const override
	{
		return TEXT("FToroSaveGameArchive");
	}

	using FArchive::operator<<; // For other overloads not overwritten in this class

	virtual FArchive& operator<<(FSoftObjectPtr& Value) override
	{
		return FArchiveUObject::SerializeSoftObjectPtr(*this, Value);
	}

	virtual FArchive& operator<<(FSoftObjectPath& Value) override
	{
		return FArchiveUObject::SerializeSoftObjectPath(*this, Value);
	}

	virtual FArchive& operator<<(FObjectPtr& Obj) override
	{
		if (UObject* InnerObject = Obj.Get())
		{
			return *this << InnerObject;
		}

		return *this;
	}

	virtual FArchive& operator<<(FWeakObjectPtr& Obj) override
	{
		if (UObject* InnerObject = Obj.Get())
		{
			return *this << InnerObject;
		}

		return *this;
	}

	virtual FArchive& operator<<(UObject*& Obj) override
	{
		if (IsValid(Obj))
		{
			FString Class = Obj->GetClass()->GetPathName();
			*this << Class;

			FToroSaveGameProxyArchive ProxyArchive(*this);

			ISavableObject::NotifyPreSave(Obj);
			Obj->Serialize(ProxyArchive);
			ISavableObject::NotifyPostSave(Obj);
		}

		return *this;
	}
};

/**
 * A memory reader used to reconstruct UObject data from a byte array.
 * Includes safety checks for class-mismatches during deserialization.
 */
class TORORUNTIME_API FToroLoadGameArchive : public FMemoryReader
{
public:

	FToroLoadGameArchive(const TArray<uint8>& InBytes)
		: FMemoryReader(InBytes, true)
	{
		Seek(0);
	}

	virtual FString GetArchiveName() const override
	{
		return TEXT("FToroLoadGameArchive");
	}

	using FArchive::operator<<; // For other overloads not overwritten in this class

	virtual FArchive& operator<<(FSoftObjectPtr& Value) override
	{
		return FArchiveUObject::SerializeSoftObjectPtr(*this, Value);
	}

	virtual FArchive& operator<<(FSoftObjectPath& Value) override
	{
		return FArchiveUObject::SerializeSoftObjectPath(*this, Value);
	}

	virtual FArchive& operator<<(FObjectPtr& Obj) override
	{
		if (UObject* InnerObject = Obj.Get())
		{
			return *this << InnerObject;
		}

		return *this;
	}

	virtual FArchive& operator<<(FWeakObjectPtr& Obj) override
	{
		if (UObject* InnerObject = Obj.Get())
		{
			return *this << InnerObject;
		}

		return *this;
	}

	virtual FArchive& operator<<(UObject*& Obj) override
	{
		if (IsValid(Obj))
		{
			FString Class;
			*this << Class;
		
			if (Class == Obj->GetClass()->GetPathName())
			{
				FToroSaveGameProxyArchive ProxyArchive(*this);

				ISavableObject::NotifyPreLoad(Obj);
				Obj->Serialize(ProxyArchive);
				ISavableObject::NotifyPostLoad(Obj);
			}
			else
			{
				SetError();
				UE_LOG(LogToroRuntime, Error, 
					TEXT("Class mismatch during load: Expected %s, got %s at byte position %lld"), 
					*Obj->GetClass()->GetPathName(), *Class, Tell()
				);
			}
		}

		return *this;
	}
};