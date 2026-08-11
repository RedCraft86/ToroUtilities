// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "SGameplayTagCombo.h"
#include "DetailsPanel/ToroStructCustomization.h"
#include "DataAssets/ToroDatabase.h"

#define STRUCT_NAME FToroDatabaseKey
class FToroDatabaseKeyDetails final : public FToroStructCustomization
{
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		FToroStructCustomization::CustomizeHeader(StructHandle, HeaderRow, CustomizationUtils);

		static const FName META_TagFilter = TEXT("TagFilter");
		const FString FilterTag = StructHandle->GetMetaData(META_TagFilter).Replace(TEXT(" "), TEXT(""));

		GET_STRUCT_PROPERTY_VAR_NS(Tag, Tag);
		HeaderRow.NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SGameplayTagCombo)
				.Filter(FilterTag)
				.PropertyHandle(Tag)
		];
	}
};
#undef STRUCT_NAME