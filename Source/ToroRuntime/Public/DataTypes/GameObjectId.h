// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "GameplayTagContainer.h"
#include "GameObjectId.generated.h"

/**
 * Persistent identity composed of a gameplay-tag group and a GUID.
 *
 * An identity is valid only when both its group and GUID are valid. The group
 * categorizes the object while the GUID distinguishes it from other objects
 * in that group.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FGameObjectId final
{
	GENERATED_BODY()

	/**
	 * Category to which the identified object belongs.
	 */
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = Identity)
	FGameplayTag Group;

	/**
	 * Unique value that distinguishes the object within its group.
	 */
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = Identity, NonTransactional)
	FGuid Guid;

	/**
	 * Invalid identity used when no identity is available.
	 */
	static const FGameObjectId EmptyId;

	/**
	 * Creates an invalid identity.
	 */
	FGameObjectId()
		: Group(FGameplayTag::EmptyTag), Guid(FGuid())
	{}

	/**
	 * Creates an identity in the specified group with a newly generated GUID.
	 * @param InGroup Group to assign to the identity.
	 */
	FGameObjectId(const FGameplayTag& InGroup)
		: Group(InGroup), Guid(FGuid::NewGuid())
	{}

	/**
	 * Creates an identity from an existing group and GUID.
	 * @param InGroup Group to assign to the identity.
	 * @param InGuid GUID to assign to the identity.
	 */
	FGameObjectId(const FGameplayTag& InGroup, const FGuid& InGuid)
		: Group(InGroup), Guid(InGuid)
	{}

	/**
	 * Clears the group and invalidates the GUID.
	 */
	void Invalidate();

	/**
	 * Assigns a newly generated GUID.
	 * @param bOnlyIfInvalid When true, preserves an existing valid GUID.
	 */
	void RegenerateGuid(const bool bOnlyIfInvalid = false);

	/**
	 * Checks whether both components of this identity are valid.
	 * @return true when both the group and GUID are valid; otherwise, false.
	 */
	[[nodiscard]] FORCEINLINE bool IsValid() const
	{
		return Group.IsValid() && Guid.IsValid();
	}

	/**
	 * Formats the identity as Group[Guid].
	 * @return String representation of this identity.
	 */
	[[nodiscard]] FString ToString() const
	{
		return FString::Printf(TEXT("%s[%s]"), *Group.ToString(), *Guid.ToString());
	}

	/**
	 * @param Other Identity to compare against.
	 * @return true when the group and GUID both match; otherwise, false.
	 */
	[[nodiscard]] FORCEINLINE bool operator==(const FGameObjectId& Other) const
	{
		return Group == Other.Group && Guid == Other.Guid;
	}

	/**
	 * @param Other Identity to compare against.
	 * @return true when either the group or GUID differs; otherwise, false.
	 */
	[[nodiscard]] FORCEINLINE bool operator!=(const FGameObjectId& Other) const
	{
		return Group != Other.Group || Guid != Other.Guid;
	}

	/**
	 * Serializes the identity through a standard archive.
	 */
	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FGameObjectId& ObjectId)
	{
		return Ar << ObjectId.Group << ObjectId.Guid;
	}

	/**
	 * Serializes the identity through a structured archive slot.
	 */
	FORCEINLINE friend void operator<<(FStructuredArchive::FSlot Slot, FGameObjectId& ObjectId)
	{
		FStructuredArchive::FRecord Record = Slot.EnterRecord();
		Record << SA_VALUE(TEXT("Group"), ObjectId.Group) << SA_VALUE(TEXT("Guid"), ObjectId.Guid);
	}

	/**
	 * Produces a combined hash of the group and GUID.
	 * @param ObjectId Identity to hash.
	 * @return Hash value for the complete identity.
	 */
	FORCEINLINE friend uint32 GetTypeHash(const FGameObjectId& ObjectId)
	{
		return HashCombine(GetTypeHash(ObjectId.Group), GetTypeHash(ObjectId.Guid));
	}
};
