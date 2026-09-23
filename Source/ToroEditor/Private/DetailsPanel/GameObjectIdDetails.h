// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "IDetailGroup.h"
#include "SGameplayTagCombo.h"
#include "DetailsPanel/ToroStructCustomization.h"
#include "DataTypes/GameObjectId.h"

#define STRUCT_NAME FGameObjectId
class FGameObjectIdDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		FToroStructCustomization::CustomizeChildren(StructHandle, StructBuilder, StructCustomizationUtils);

		static const FName META_Categories = TEXT("Categories");
		const FString FilterTag = StructHandle->GetMetaData(META_Categories).Replace(TEXT(" "), TEXT(""));

		GET_STRUCT_PROPERTY_VAR_NS(Group, Group)

		IDetailGroup& StructGroup = StructBuilder.AddGroup(TEXT("ObjectId"), StructHandle->GetPropertyDisplayName());
		StructGroup.HeaderRow().NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SGameplayTagCombo)
				.Filter(FilterTag)
				.PropertyHandle(Group)
		];

		StructGroup.AddPropertyRow(GET_STRUCT_PROPERTY_NS(Guid).ToSharedRef());
	}
};
#undef STRUCT_NAME