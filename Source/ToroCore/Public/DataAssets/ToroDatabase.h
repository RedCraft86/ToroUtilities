// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ToroDataAsset.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "ToroDatabase.generated.h"

/**
 * A wrapper of FGameplayTag used in UToroDatabase.
 * While not necessary, it is required for the 
 * details customization allowing filtering to work.
 */
USTRUCT(BlueprintInternalUseOnly, meta = (Hidden))
struct TOROCORE_API FToroDatabaseKey final
{
	GENERATED_BODY()

private:

	friend class FToroDatabaseKeyDetails;

	/** 
	 * The underlying GameplayTag representing this key. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Entry, meta = (AllowPrivateAccess = true))
		FGameplayTag Tag;

public:

	FToroDatabaseKey() {}
	FToroDatabaseKey(const FGameplayTag& InTag) : Tag(InTag) {}

	FORCEINLINE bool IsValid() const { return Tag.IsValid(); }
	FORCEINLINE FName ToName() const { return Tag.GetTagName(); }
	FORCEINLINE FString ToString() const { return Tag.ToString(); }

	FORCEINLINE const FGameplayTag& operator*() const { return Tag; }
	FORCEINLINE explicit operator FGameplayTag() const { return Tag; }

	FORCEINLINE bool operator==(const FToroDatabaseKey& Other) const { return Tag == Other.Tag; }
	FORCEINLINE bool operator!=(const FToroDatabaseKey& Other) const { return Tag != Other.Tag; }

	FORCEINLINE void operator=(const FNativeGameplayTag& Other) { Tag = Other.GetTag(); }
	FORCEINLINE void operator=(const FGameplayTag& Other) { Tag = Other; }

	FORCEINLINE friend uint32 GetTypeHash(const FToroDatabaseKey& Key)
	{
		return GetTypeHash(Key.Tag);
	}

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FToroDatabaseKey& Key)
	{
		return Ar << Key.Tag;
	}

	FORCEINLINE friend void operator<<(FStructuredArchive::FSlot Slot, FToroDatabaseKey& Key)
	{
		Slot << Key.Tag;
	}
};

/**
 * An abstract base struct to be used alongside ToroDatabase assets.
 * This is a polymorphic struct that provides a standardized interface 
 * for data validation within the database asset.
 */
USTRUCT(BlueprintInternalUseOnly, meta = (Hidden))
struct TOROCORE_API FToroDatabaseEntry
{
	GENERATED_BODY()

	virtual ~FToroDatabaseEntry() = default;

	virtual bool IsValid() const { return false; }
};

/**
 * An abstract base class for assets acting as collections of DatabaseEntry structs.
 * Indexed by GameplayTags, this system is ideal for Ability Lists or Quest Databases.
 * @note Inheriting classes MUST use the templated protected constructor to register their expected data type.
 */
UCLASS(Abstract, PrioritizeCategories = (Asset))
class TOROCORE_API UToroDatabase : public UToroDataAsset
{
	GENERATED_BODY()

	friend class FToroDatabaseDetails;

public:

	/**
	 * Inheriting classes MUST use the templated constructor to set their expected data type.
	 */
	UToroDatabase() {}

	/** 
	 * Checks if a specific key exists within the database map.
	 */
	UFUNCTION(BlueprintPure, Category = Database)
	bool DoesKeyExist(const FGameplayTag& Key) const
	{
		return Key.IsValid() && Entries.Contains(Key);
	}

	/**
	 * Retrieves a database entry as an InstancedStruct.
	 * @param Key The GameplayTag associated with the entry.
	 * @return An Instanced struct to the data or an empty one if key is missing.
	 */
	UFUNCTION(BlueprintPure, Category = Database)
	FInstancedStruct GetValue(const FGameplayTag& Key) const
	{
		if (!DoesKeyExist(Key))
		{
			return FInstancedStruct();
		}

		const FInstancedStruct* FoundValue = Entries.Find(Key);
		return FoundValue->IsValid() ? *FoundValue : FInstancedStruct();
	}

	/**
	 * Retrieves a type-safe pointer to a database entry.
	 * @tparam StructType The expected struct type (must derive from FToroDatabaseEntry and be a USTRUCT).
	 * @param Key The GameplayTag associated with the entry.
	 * @return A const pointer to the struct data, or nullptr if the key is missing or type is mismatched.
	 */
	template<typename StructType>
	const StructType* GetValue(const FGameplayTag& Key) const
	{
		static_assert(TIsDerivedFrom<StructType, FToroDatabaseEntry>::IsDerived, 
			"StructType must derive from FToroDatabaseEntry and be a USTRUCT");

		const UScriptStruct* OutType = TBaseStructure<StructType>::Get();
		if (!ensureAlwaysMsgf(OutType, TEXT("T is not a USTRUCT type"))
			|| !ensureAlwaysMsgf(RootStruct, TEXT("RootStruct is null")))
		{
			return nullptr;
		}

		if (!ensureAlwaysMsgf(OutType == RootStruct || OutType->IsChildOf(RootStruct),
			TEXT("T (%s) is not RootStruct (%s) or a child of it"), 
			*OutType->GetName(), *RootStruct->GetName()))
		{
			return nullptr;
		}

		if (!DoesKeyExist(Key))
		{
			return nullptr;
		}

		const FInstancedStruct* FoundValue = Entries.Find(Key);
		return FoundValue->IsValid() ? FoundValue->GetPtr<StructType>() : nullptr;
	}

	/** 
	 * Validates if a key is eligible for use in this database.
	 * By default, requires the key to be a sub-tag of the RootTag.
	 * <pre>
	 * e.g, if RootTag is 'Quest'
	 *    'None'              is invalid
	 *    'Objective'         is invalid
	 *    'Quest'             is invalid
	 *    'Quest.WalkForward' is valid
	 * </pre>
	 * @param Key The tag to check.
	 * @return True if the key follows the database's tag hierarchy rules.
	 */
	virtual bool IsValidKey(const FGameplayTag& Key) const
	{
		return Key.IsValid() && Key.MatchesTag(RootTag) && Key != RootTag;
	}

	/** 
	 * @return Direct access to the internal map of all database entries. 
	 */
	const TMap<FToroDatabaseKey, FInstancedStruct>& GetEntries() const { return Entries; }

protected:

	/**
	 * Specialized constructor for child classes to define their data and tag constraints.
	 * @param TagType The root GameplayTag that all keys in this database must descend from.
	 * @param StructType The base struct type all entries in this database must inherit from.
	 */
	UToroDatabase(const FGameplayTag& TagType, const UScriptStruct* StructType)
		: RootTag(TagType), RootStruct(StructType)
	{
		ensureAlwaysMsgf(StructType && StructType->IsChildOf<FToroDatabaseEntry>(), 
			TEXT("StructType (%s) must derive from FToroDatabaseEntry"), *GetNameSafe(StructType));
	}

	/**
	 * The required parent tag for all entries in this database (e.g., 'Quest', 'Ability'). 
	 */
	UPROPERTY(VisibleAnywhere, Category = Asset, meta = (DisplayPriority = 0))
		FGameplayTag RootTag;

	/** 
	 * The base ScriptStruct type that all entries in the 'Entries' map must conform to. 
	 */
	UPROPERTY(VisibleAnywhere, Category = Asset, meta = (DisplayPriority = 0))
		TObjectPtr<const UScriptStruct> RootStruct;

	/** 
	 * The core data map. Uses TInstancedStruct for polymorphism while maintaining the efficiency of a single TMap.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Data, TextExportTransient, meta = (ForceInlineRow))
		TMap<FToroDatabaseKey, FInstancedStruct> Entries;

#if WITH_EDITOR
	virtual void ValidateData() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
