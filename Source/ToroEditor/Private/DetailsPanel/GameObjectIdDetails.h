// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"

#include "DataTypes/GameObjectId.h"

#define STRUCT_NAME FGameObjectId
class FGameObjectIdDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		IDetailGroup& StructGroup = StructBuilder.AddGroup(TEXT("GameObjectId"), StructHandle->GetPropertyDisplayName());

		StructGroup.HeaderProperty(GET_STRUCT_PROPERTY_NS(Group).ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName());

		StructGroup.AddPropertyRow(GET_STRUCT_PROPERTY_NS(Guid).ToSharedRef());
	}
};
#undef STRUCT_NAME