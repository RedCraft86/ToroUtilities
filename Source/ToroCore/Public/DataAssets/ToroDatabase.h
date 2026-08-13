// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

// ReSharper disable CppNonExplicitConversionOperator
#pragma once

#include "ToroDataAsset.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "ToroDatabase.generated.h"

/**
 * A struct to be used alongside UToroDatabase for data keys.
 */
USTRUCT(BlueprintInternalUseOnly, meta = (Hidden))
struct TOROCORE_API FToroDatabaseKey final
{
	GENERATED_BODY()

private:
#if WITH_EDITOR
	friend class FToroDatabaseKeyDetails;
#endif

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

	FORCEINLINE operator FGameplayTag&() { return Tag; }
	FORCEINLINE operator const FGameplayTag&() const { return Tag; }

	FORCEINLINE bool operator==(const FToroDatabaseKey& Other) const { return Tag == Other.Tag; }
	FORCEINLINE bool operator!=(const FToroDatabaseKey& Other) const { return Tag != Other.Tag; }

	FORCEINLINE void operator=(const FGameplayTag& Other) { Tag = Other; }

	FORCEINLINE friend FArchive& operator<<(FArchive& Ar, FToroDatabaseKey& Key)
	{
		return Ar << Key.Tag;
	}

	FORCEINLINE friend void operator<<(FStructuredArchive::FSlot Slot, FToroDatabaseKey& Key)
	{
		Slot << Key.Tag;
	}

	FORCEINLINE friend uint32 GetTypeHash(const FToroDatabaseKey& Key)
	{
		return GetTypeHash(Key.Tag);
	}
};

/**
 * An abstract base struct to be used alongside UToroDatabase for data values.
 */
USTRUCT(BlueprintInternalUseOnly, meta = (Hidden))
struct TOROCORE_API FToroDatabaseEntry
{
	GENERATED_BODY()

	virtual ~FToroDatabaseEntry() = default;

	virtual bool IsValid() const { return false; }
};

/**
 * An abstract base class for assets acting as collections of DatabaseEntry structs indexed by GameplayTags.
 * @note Inheriting classes MUST use the protected constructor to register their expected data type.
 */
UCLASS(Abstract, PrioritizeCategories = (Asset))
class TOROCORE_API UToroDatabase : public UToroDataAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FToroDatabaseDetails;
#endif
public:

	/**
	 * Inheriting classes MUST use the templated constructor to set their expected data type.
	 */
	UToroDatabase() {}

	/**
	 * Checks if a specific key exists within the database map.
	 */
	UFUNCTION(BlueprintPure, Category = Database)
		bool DoesKeyExist(const FGameplayTag& Key) const;

	/**
	 * Retrieves a database entry as an InstancedStruct.
	 * @param Key The GameplayTag associated with the entry.
	 * @return An Instanced struct to the data or an empty one if key is missing.
	 */
	UFUNCTION(BlueprintPure, Category = Database)
		FInstancedStruct GetValue(const FGameplayTag& Key) const;

	/**
	 * Validates if a key is eligible for use in this database.
	 * By default, requires the key to be a sub-tag of the RootTag.
	 * <pre>
	 * e.g., if RootTag is 'Quest'
	 * 'None'              is invalid
	 * 'Character'         is invalid
	 * 'Quest'             is invalid
	 * 'Quest.WalkForward' is valid
	 * </pre>
	 * @param Key The tag to check.
	 * @return True if the key follows the database's tag hierarchy rules.
	 */
	virtual bool IsValidKey(const FGameplayTag& Key) const;

	/**
	 * @return Direct access to the internal map of all database entries.
	 */
	const TMap<FToroDatabaseKey, FInstancedStruct>& GetEntries() const { return Entries; }

	/**
	 * Retrieves a type-safe pointer to a database entry.
	 * @tparam StructType The expected struct type (must derive from FToroDatabaseEntry and be a USTRUCT).
	 * @param Key The GameplayTag associated with the entry.
	 * @return A const pointer to the struct data, or nullptr if the key is missing or type is mismatched.
	 */
	template<typename StructType>
	const StructType* GetValue(const FGameplayTag& Key) const
	{
		static_assert(TIsDerivedFrom<StructType, FToroDatabaseEntry>::Value,
			"StructType must derive from FToroDatabaseEntry and be a USTRUCT");

		const UScriptStruct* OutType = TBaseStructure<StructType>::Get();
		if (!ensureAlwaysMsgf(OutType, TEXT("T is not a USTRUCT type"))
			|| !ensureAlwaysMsgf(RootStruct, TEXT("RootStruct is null")))
		{
			return nullptr;
		}

		if (!ensureAlwaysMsgf(OutType == RootStruct || OutType->IsChildOf(RootStruct),
			TEXT("T (%s) is not RootStruct (%s) or a child of it"), *OutType->GetName(), *RootStruct->GetName()))
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

protected:

	/**
	 * Specialized constructor for child classes to define their data and tag constraints.
	 * @param TagType The root GameplayTag that all keys in this database must descend from.
	 * @param StructType The base struct type all entries in this database must inherit from.
	 */
	UToroDatabase(const FGameplayTag& TagType, const UScriptStruct* StructType);

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
