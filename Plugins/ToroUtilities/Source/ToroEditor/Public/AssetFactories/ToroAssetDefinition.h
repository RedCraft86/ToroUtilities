// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "AssetDefinitionDefault.h"
#include "MiscObjects/ToroDataAsset.h"
#include "ToroAssetDefinition.generated.h"

UCLASS(Abstract)
class TOROEDITOR_API UToroAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const TArray Categories{FAssetCategoryPath(INVTEXT("Data Assets"))};
		return Categories;
	}

	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		FText OutText = FText::GetEmpty();
		if (const UToroDataAsset* DA = Cast<UToroDataAsset>(AssetData.GetAsset()))
		{
			OutText = DA->GetDescription();
		}

		if (const FText SuperText = Super::GetAssetDescription(AssetData); !SuperText.IsEmptyOrWhitespace())
		{
			OutText = FText::Format(INVTEXT("{0}\n{1}"), OutText, SuperText);
		}

		return OutText;
	}
};