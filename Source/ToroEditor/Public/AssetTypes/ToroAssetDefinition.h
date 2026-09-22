// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "AssetDefinitionDefault.h"
#include "MiscObjects/ToroDataAsset.h"
#include "ToroAssetDefinition.generated.h"

/**
 * Base asset definition for data assets provided by ToroUtilities.
 */
UCLASS(Abstract)
class TOROEDITOR_API UToroAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:

	/**
	 * Gets the Content Browser categories containing this asset type.
	 */
	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const TArray Categories { FAssetCategoryPath(INVTEXT("ToroUtilities")) };
		return Categories;
	}

	/**
	 * Gets the identifying color used for this asset type.
	 */
	virtual FLinearColor GetAssetColor() const override
	{
		return FLinearColor::Green;
	}

	/**
	 * Gets the display name used for this asset type.
	 */
	virtual FText GetAssetDisplayName() const override
	{
		return INVTEXT("Unknown Asset");
	}

	/**
	 * Gets the description displayed for an asset instance.
	 * @param Asset Asset metadata for the instance being described.
	 */
	virtual FText GetAssetDescription(const FAssetData& Asset) const override
	{
		return FText::GetEmpty();
	}

	/**
	 * Gets the class represented by this asset definition.
	 */
	virtual TSoftClassPtr<UObject> GetAssetClass() const override
	{
		return UToroDataAsset::StaticClass();
	}
};