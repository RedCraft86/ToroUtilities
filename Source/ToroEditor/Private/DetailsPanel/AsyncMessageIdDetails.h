// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "AsyncMessageId.h"

/**
 * @warning This customizes a struct in the <c>AsyncMessageSystem</c> plugin. It may break with plugin changes.
 */
class FAsyncMessageIdDetails final : public FToroStructCustomization
{
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		FToroStructCustomization::CustomizeHeader(StructHandle, HeaderRow, CustomizationUtils);

		const TSharedPtr<IPropertyHandle> MsgTag = StructHandle->GetChildHandle(TEXT("InternalMessageTag"));
		MsgTag->MarkHiddenByCustomization();

		static const FName META_Categories = TEXT("Categories");
		const FString FilterTag = StructHandle->GetMetaData(META_Categories).Replace(TEXT(" "), TEXT(""));

		// Display inner property as the struct itself
		HeaderRow.NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SGameplayTagCombo)
				.Filter(FilterTag)
				.PropertyHandle(MsgTag)
		];
	}
};