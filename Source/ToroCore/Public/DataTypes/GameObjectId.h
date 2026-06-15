// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "GameplayTagContainer.h"
#include "GameObjectId.generated.h"

/**
 * A semantically grouped unique identifier used to globally identify game objects.
 * 
 * This struct combines a FGameplayTag (representing a category or 'Group') with a unique FGuid.
 * This dual approach allows for efficient filtering by group while maintaining absolute 
 * uniqueness for individual instances.
 *
 * Features:
 * - Fully compatible with the Unreal Reflection system and Blueprints.
 * - Supports SaveGame serialization for persistent object tracking.
 * - Implements GetTypeHash, allowing it to be used as a key in TMaps and TSets.
 * - NonTransactional GUID prevents ID regeneration during Undo/Redo operations in the editor.
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FGameObjectId final
{
	GENERATED_BODY()

	/** The category or group this object belongs to (e.g., "Item.Weapon", "Unit.Enemy"). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectId, SaveGame)
		FGameplayTag Group;

	/** The unique instance identifier. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameObjectId, SaveGame, NonTransactional)
		FGuid Guid;

	FGameObjectId(): Group(FGameplayTag::EmptyTag), Guid(FGuid()) {}
	FGameObjectId(const FGameplayTag& InGroup): Group(InGroup), Guid(FGuid::NewGuid()) {}
	FGameObjectId(const FGameplayTag& InGroup, const FGuid& InGuid): Group(InGroup), Guid(InGuid) {}

	/** Returns a reference to a static invalid/empty ID. */
	static const FGameObjectId& GetEmpty();

	/** Clears the Group and invalidates the GUID. */
	void Invalidate();

	/** 
	 * Generates a new GUID for this Game Object Id.
	 * @param bOnlyIfInvalid If true, a new GUID will only be generated if the current one is already invalid.
	 */
	void RegenerateGuid(const bool bOnlyIfInvalid = false);

	/** Returns true if the Group is valid and the GUID is properly initialized. */
	FORCEINLINE bool IsValid() const
	{
		return Group.IsValid() && Guid.IsValid();
	}

	/** Returns a string representation in the format "Group_GUID". */
	FORCEINLINE FString ToString() const
	{
		return FString::Printf(TEXT("%s_%s"), *Group.ToString(), *Guid.ToString());
	}

	FORCEINLINE bool operator==(const FGameObjectId& Other) const
	{
		return Group == Other.Group && Guid == Other.Guid;
	}

	FORCEINLINE bool operator!=(const FGameObjectId& Other) const
	{
		return Group != Other.Group || Guid != Other.Guid;
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGameObjectId& InObjectID)
	{
		Ar << InObjectID.Group;
		Ar << InObjectID.Guid;
		return Ar;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FGameObjectId& InObjectID)
	{
		return HashCombine(GetTypeHash(InObjectID.Group), GetTypeHash(InObjectID.Guid));
	}
};
