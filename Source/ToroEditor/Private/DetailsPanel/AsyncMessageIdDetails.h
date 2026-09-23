// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "SGameplayTagCombo.h"
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