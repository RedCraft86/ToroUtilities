// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ToroDataAsset.h"
#include "ToroDatabase.generated.h"

/**
 * Base structure for all entries within a ToroDatabase.
 * Designed to work with the editor to provide readable labels while 
 * enforcing strict data validation.
 */
USTRUCT(BlueprintInternalUseOnly)
struct TOROCORE_API FDatabaseEntryBase
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Ed, AdvancedDisplay, meta = (EditCondition = false, EditConditionHides))
		FString EdLabel = FString();
#endif

	FDatabaseEntryBase() {}
	virtual ~FDatabaseEntryBase() = default;

	virtual bool IsValidData() const PURE_VIRTUAL(FDatabaseEntry::IsValidData, return false;)
};

/**
 * An abstract base class for assets acting as collections of FDatabaseEntryBase items.
 * Intended for use in systems like Inventory, Ability Lists, or Quest Databases.
 */
UCLASS(Abstract, PrioritizeCategories = (Asset))
class TOROCORE_API UToroDatabase : public UToroDataAsset
{
	GENERATED_BODY()

public:

	UToroDatabase()
	{
		DisplayName = INVTEXT("Generic Database");
		Description = INVTEXT("This is a generic database");
	}

#if WITH_EDITOR
	/** 
	 * Retrieves the current health of the database entries.
	 * @return An FIntPoint where X = Total Entry Count and Y = Number of Valid Entries.
	 */
	virtual FIntPoint GetEntryCount() const PURE_VIRTUAL(UToroDatabase::GetEntryCount, return FIntPoint::ZeroValue;)
#endif
};
