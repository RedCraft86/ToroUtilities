// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#pragma once

#include "GameplayTagContainer.h"
#include "GameObjectId.generated.h"

/**
 * A semantically grouped unique identifier used to globally identify important game objects.
 * Uses a combination of FGameplayTag for grouping and FGuid for instance separation.
 * 
 * <b>Ideal for:</b>
 * - Character Tracking
 * - Save Systems
 */
USTRUCT(BlueprintType)
struct TOROCORE_API FGameObjectId final
{
	GENERATED_BODY()

	/** An empty identity for internal uses. */
	static const FGameObjectId EmptyId;

	/** The category or group this object belongs to (e.g., <c>Interactive.Door</c> or <c>Character.SomeEnemy</c>). */
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = Identity)
		FGameplayTag Group;

	/** Separation for instances within the group (i.e. there may be multiple instances of <c>Interactive.Door</c>) */
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite, Category = Identity, NonTransactional)
		FGuid Guid;

	FGameObjectId(): Group(FGameplayTag::EmptyTag), Guid(FGuid()) {}
	FGameObjectId(const FGameplayTag& InGroup): Group(InGroup), Guid(FGuid::NewGuid()) {}
	FGameObjectId(const FGameplayTag& InGroup, const FGuid& InGuid): Group(InGroup), Guid(InGuid) {}

	/** Gets an Object Identity from the specified target if IObjectIdentity is implemented. */
	static FGameObjectId Get(const UObject* Target);

	/** Clears the group and invalidates the guid on this identifier. */
	void Invalidate();

	/**
	 * Generates a new Guid for this identity.
	 * @param bOnlyIfInvalid If true, only generate the guid if the current one is invalid.
	 */
	void RegenerateGuid(const bool bOnlyIfInvalid = false);

	/** Checks if Group and Guid are both valid in this identity. */
	FORCEINLINE bool IsValid() const
	{
		return Group.IsValid() && Guid.IsValid();
	}

	/** This identity in string format as <c>GroupTag[Instance]</c> */
	FString ToString() const
	{
		return FString::Printf(TEXT("%s[%s]"), *Group.ToString(), *Guid.ToString());
	}

	FORCEINLINE bool operator==(const FGameObjectId& Other) const
	{
		return Group == Other.Group && Guid == Other.Guid;
	}

	FORCEINLINE bool operator!=(const FGameObjectId& Other) const
	{
		return Group != Other.Group || Guid != Other.Guid;
	}

	friend void operator<<(FStructuredArchive::FSlot Slot, FGameObjectId& ObjectId)
	{
		FStructuredArchive::FRecord Record = Slot.EnterRecord();
		Record << SA_VALUE(TEXT("Group"), ObjectId.Group) << SA_VALUE(TEXT("Guid"), ObjectId.Guid);
	}

	friend FArchive& operator<<(FArchive& Ar, FGameObjectId& ObjectId)
	{
		return Ar << ObjectId.Group << ObjectId.Guid;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FGameObjectId& ObjectId)
	{
		return HashCombine(GetTypeHash(ObjectId.Group), GetTypeHash(ObjectId.Guid));
	}
};
