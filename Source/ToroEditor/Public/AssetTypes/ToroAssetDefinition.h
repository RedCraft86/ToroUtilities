// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "AssetDefinitionDefault.h"
#include "DataAssets/ToroDatabase.h"
#include "ToroAssetDefinition.generated.h"

/**
 * Base class for custom asset definitions in the ToroUtilities framework.
 */
UCLASS(Abstract)
class TOROEDITOR_API UToroAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const TArray Categories { FAssetCategoryPath(INVTEXT("ToroUtilities")) };
		return Categories;
	}

	virtual FLinearColor GetAssetColor() const override { return FLinearColor::Green; }
	virtual FText GetAssetDisplayName() const override { return INVTEXT("Unknown Database"); }
	virtual FText GetAssetDescription(const FAssetData& Asset) const override { return FText::GetEmpty(); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UToroDatabase::StaticClass(); }
};
