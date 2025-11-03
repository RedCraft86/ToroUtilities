// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "GameplayTagContainer.h"
#include "GameObjectID.generated.h"

/**
 * Represents a unique identifier for a game object, combining a gameplay tag group
 * and a GUID for precise distinction between instances. 
 *
 * This struct allows grouping through tags (e.g., "Character.John", "Puzzle.KeyCode")
 * and unique instance tracking via a GUID.
 * 
 * Supports serialization, hashing, and equality comparison for use in maps, sets, and save systems.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FGameObjectID
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectID, SaveGame)
		FGameplayTag Group;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectID, SaveGame, NonTransactional)
		FGuid Guid;

	FGameObjectID(): Group(FGameplayTag::EmptyTag), Guid(FGuid()) {}
	FGameObjectID(const FGameplayTag& InGroup): Group(InGroup), Guid(FGuid::NewGuid()) {}
	FGameObjectID(const FGameplayTag& InGroup, const FGuid& InGuid): Group(InGroup), Guid(InGuid) {}

	bool IsValid() const { return Group.IsValid() && Guid.IsValid(); }
	FString ToString() const { return FString::Printf(TEXT("%s_%s"), *Group.ToString(), *Guid.ToString()); }
	void RegenerateGuid() { Guid = FGuid::NewGuid(); }
	void Invalidate()
	{
		Group = FGameplayTag::EmptyTag;
		Guid.Invalidate();
	}


	FORCEINLINE bool operator==(const FGameObjectID& Other) const
	{
		return Group == Other.Group && Guid == Other.Guid;
	}

	FORCEINLINE bool operator!=(const FGameObjectID& Other) const
	{
		return Group != Other.Group || Guid != Other.Guid;
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGameObjectID& InObjectID)
	{
		Ar << InObjectID.Group;
		Ar << InObjectID.Guid;
		return Ar;
	}
	
	[[nodiscard]] FORCEINLINE friend uint32 GetTypeHash(const FGameObjectID& InObjectID)
	{
		return HashCombine(GetTypeHash(InObjectID.Group), GetTypeHash(InObjectID.Guid));
	}
};