// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/DataAsset.h"
#include "ToroDataAsset.generated.h"

/**
 * Base struct for database entries in the ToroUtilities architecture.
 * This is a polymorphic struct that serves as a foundation for specific database entry types.
 * 
 * Usage Pattern:
 * - Derive from this struct to create specific entry types (e.g., FAchievementEntry, FWeatherEntry)
 * - Fill the EdLabel field through Database's RefreshData function to report back the validity of this entry
 */
USTRUCT(BlueprintInternalUseOnly)
struct TOROCORE_API FDatabaseEntry
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Label, AdvancedDisplay, meta = (EditCondition = 0, EditConditionHides))
		FString EdLabel = TEXT("");
#endif

	FDatabaseEntry() {}
	virtual ~FDatabaseEntry() = default;

	virtual bool IsValidData() const PURE_VIRTUAL(FDatabaseEntry::IsValidData, return false;)
};

/**
 * Abstract base class for data assets in the ToroUtilities architecture.
 * Extends UDataAsset with editor-specific functionality for data management and validation.
 */
UCLASS(Abstract)
class TOROCORE_API UToroDataAsset : public UDataAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
public:
	virtual void RefreshData() PURE_VIRTUAL(UToroDataAsset::RefreshData,)
	virtual FText GetDescription() const PURE_VIRTUAL(UToroDataAsset::GetDescription, return FText::GetEmpty();)
protected:
	virtual void PostLoad() override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

/**
 * Abstract base class for database-style data assets that manage collections of entries. Intended to be a
 * replacement for Data Tables with better C++ type safety and allowing FGameplayTag as keys instead of raw FName.
 * This class is designed for data assets that contain arrays or maps of FDatabaseEntry-derived structs.
 */
UCLASS(Abstract)
class TOROCORE_API UToroDatabase : public UToroDataAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
public:
	/* Max = Total entries | Min = Valid entries */
	virtual FIntPoint GetStatistics() const PURE_VIRTUAL(UToroDatabase::GetStatistics, return FIntPoint::ZeroValue;)
	virtual FText GetDescription() const override;
#endif
};
