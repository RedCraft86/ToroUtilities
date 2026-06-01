// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "AssetDefinitionDefault.h"
#include "DataAssets/ToroDatabase.h"
#include "ToroAssetDefinition.generated.h"

UCLASS(Abstract)
class TOROEDITOR_API UToroAssetDefinition : public UAssetDefinitionDefault
{
	GENERATED_BODY()

public:

	virtual TConstArrayView<FAssetCategoryPath> GetAssetCategories() const override
	{
		static const TArray Categories{FAssetCategoryPath(INVTEXT("ToroUtilities"))};
		return Categories;
	}
};

UCLASS(Abstract)
class TOROEDITOR_API UToroDatabaseDefinition : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FText GetAssetDescription(const FAssetData& AssetData) const override
	{
		const FText SuperText = Super::GetAssetDescription(AssetData);
		if (const UToroDatabase* DB = Cast<UToroDatabase>(AssetData.GetAsset()))
		{
			const FIntPoint Count = DB->GetEntryCount();
			if (Count.X > 0 || Count.Y > 0)
			{
				FText Statistics = FText::GetEmpty();
				if (Count.X == Count.Y)
				{
					Statistics = FText::Format(INVTEXT("{0} Valid Entries"), Count.X);
				}
				else
				{
					const int32 Valid = Count.GetMin();
					const int32 Total = Count.GetMax();
					Statistics = FText::Format(INVTEXT(
						"{0} Entries | {1} Valid | {2} Invalid"),
						Total, Valid, Total - Valid);
				}

				return SuperText.IsEmptyOrWhitespace() ? Statistics 
					: FText::Format(INVTEXT("{0}\n{1}"), SuperText, Statistics);
			}
		}

		return SuperText;
	}
};