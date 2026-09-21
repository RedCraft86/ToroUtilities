// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "GameplayTagContainer.h"
#include "ToroActorId.generated.h"

/**
 * Persistent identity composed of a gameplay-tag group and a GUID.
 *
 * An identity is valid only when both its group and GUID are valid. The group
 * categorizes the object while the GUID distinguishes it from other objects
 * in that group.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FToroActorId final
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
	static const FToroActorId EmptyId;

	FToroActorId()
		: Group(FGameplayTag::EmptyTag), Guid(FGuid())
	{}

	FToroActorId(const FGameplayTag& InGroup)
		: Group(InGroup), Guid(FGuid::NewGuid())
	{}

	FToroActorId(const FGameplayTag& InGroup, const FGuid& InGuid)
		: Group(InGroup), Guid(InGuid)
	{}

	FORCEINLINE bool operator==(const FToroActorId& Other) const
	{
		return Group == Other.Group && Guid == Other.Guid;
	}

	FORCEINLINE bool operator!=(const FToroActorId& Other) const
	{
		return Group != Other.Group || Guid != Other.Guid;
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FToroActorId& ObjectId)
	{
		return Ar << ObjectId.Group << ObjectId.Guid;
	}

	FORCEINLINE friend void operator<<(FStructuredArchive::FSlot Slot, FToroActorId& ObjectId)
	{
		FStructuredArchive::FRecord Record = Slot.EnterRecord();
		Record << SA_VALUE(TEXT("Group"), ObjectId.Group) << SA_VALUE(TEXT("Guid"), ObjectId.Guid);
	}

	/**
	 * Retrieves the Actor Id from an actor through the IToroActor interface.
	 * @param Target Actor to get the Id from. Must implement IToroActor interface.
	 * @return A valid Id or an empty one if a valid one wasn't obtained.
	 */
	[[nodiscard]] static FToroActorId Get(const AActor* Target);

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
	FORCEINLINE bool IsValid() const
	{
		return Group.IsValid() && Guid.IsValid();
	}

	/**
	 * Formats the identity as Group[Guid].
	 * @return String representation of this identity.
	 */
	FString ToString() const
	{
		return FString::Printf(TEXT("%s[%s]"), *Group.ToString(), *Guid.ToString());
	}

	/**
	 * Produces a combined hash of the group and GUID.
	 * @param ObjectId Identity to hash.
	 * @return Hash value for the complete identity.
	 */
	FORCEINLINE friend uint32 GetTypeHash(const FToroActorId& ObjectId)
	{
		return HashCombine(GetTypeHash(ObjectId.Group), GetTypeHash(ObjectId.Guid));
	}
};
