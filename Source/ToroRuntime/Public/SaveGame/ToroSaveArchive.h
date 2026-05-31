// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToroRuntime.h"
#include "ISavableObject.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

struct TORORUNTIME_API FToroSaveGameProxyArchive : FObjectAndNameAsStringProxyArchive
{
	FToroSaveGameProxyArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		ArIsSaveGame = true;
		ArNoDelta = true;
	}
};

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
		
			if (Class.Equals(Obj->GetClass()->GetPathName()))
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
					TEXT("Class mismatch: Expected %s, got %s at archive position %lld"), 
					*Obj->GetClass()->GetPathName(), *Class, Tell()
				);
			}
		}

		return *this;
	}
};