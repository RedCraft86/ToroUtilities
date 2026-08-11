// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

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
		FToroStructCustomization::CustomizeChildren(StructHandle, StructBuilder, StructCustomizationUtils);

		IDetailGroup& StructGroup = StructBuilder.AddGroup(TEXT("ObjectId"), StructHandle->GetPropertyDisplayName());

		GET_STRUCT_PROPERTY_VAR_NS(Group, Group)

		ForwardMetadata(Group);

		StructGroup.HeaderProperty(Group.ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName());

		StructGroup.AddPropertyRow(GET_STRUCT_PROPERTY_NS(Guid).ToSharedRef());
	}
};
#undef STRUCT_NAME