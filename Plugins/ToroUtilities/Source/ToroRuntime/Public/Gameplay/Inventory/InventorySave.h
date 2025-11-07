// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "InventoryAsset.h"
#include "InventorySave.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryAssetPath
{
	GENERATED_BODY()

	UPROPERTY(Transient) FSoftObjectPath Path;

	FInventoryAssetPath(): Path(nullptr) {}
	FInventoryAssetPath(const TSoftObjectPtr<UInventoryAsset>& InAsset)
		: Path(InAsset.ToSoftObjectPath())
	{}
	
	void Reset() { Path.Reset(); }
	bool IsValid() const { return !Path.IsNull(); }

	TSoftObjectPtr<UInventoryAsset> ToAsset() const
	{
		return TSoftObjectPtr<UInventoryAsset>(Path);
	}
 
	FORCEINLINE bool operator==(const FInventoryAssetPath& Other) const
	{
		return Path == Other.Path;
	}
 
	FORCEINLINE bool operator!=(const FInventoryAssetPath& Other) const
	{
		return Path != Other.Path;
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryAssetPath& Data)
	{
		FArchiveUObject::SerializeSoftObjectPath(Ar, Data.Path);
		return Ar;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FInventoryAssetPath& Data)
	{
		return GetTypeHash(Data.Path);
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryArchiveSave
{
	GENERATED_BODY()

	UPROPERTY(Transient) TArray<FInventoryAssetPath> Paths;

	FInventoryArchiveSave(): Paths({}) {}
	FInventoryArchiveSave(const TArray<TSoftObjectPtr<UInventoryAsset>>& InArchives)
	{
		Paths.Reserve(InArchives.Num());
		for (const TSoftObjectPtr<UInventoryAsset>& Archive : InArchives)
		{
			Paths.Emplace(FInventoryAssetPath(Archive));
		}
	}

	void Empty() { Paths.Empty(); }

	TArray<TSoftObjectPtr<UInventoryAsset>> ToInventoryArchives() const
	{
		TArray<TSoftObjectPtr<UInventoryAsset>> Result;
		Result.Reserve(Paths.Num());
		for (const FInventoryAssetPath& Path : Paths)
		{
			if (Path.IsValid()) Result.Add(Path.ToAsset());
		}
		return Result;
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryArchiveSave& Data)
	{
		Ar << Data.Paths;
		return Ar;
	}
};

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FInventoryItemSave
{
	GENERATED_BODY()

	UPROPERTY(Transient) TMap<FInventoryAssetPath, uint8> Paths;

	FInventoryItemSave(): Paths({}) {}
	FInventoryItemSave(const TMap<TSoftObjectPtr<UInventoryAsset>, uint8>& InSlots)
	{
		Paths.Reserve(InSlots.Num());
		for (const TPair<TSoftObjectPtr<UInventoryAsset>, uint8>& Path : InSlots)
		{
			Paths.Emplace(FInventoryAssetPath(Path.Key), Path.Value);
		}
	}

	TMap<TSoftObjectPtr<UInventoryAsset>, uint8> ToInventoryItems() const
	{
		TMap<TSoftObjectPtr<UInventoryAsset>, uint8> Result;
		Result.Reserve(Paths.Num());
		for (const TPair<FInventoryAssetPath, uint8>& Slot : Paths)
		{
			if (Slot.Key.IsValid() && Slot.Value != 0)
			{
				Result.Add(Slot.Key.ToAsset(), Slot.Value);
			}
		}
		return Result;
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FInventoryItemSave& Data)
	{
		Ar << Data.Paths;
		return Ar;
	}
};
