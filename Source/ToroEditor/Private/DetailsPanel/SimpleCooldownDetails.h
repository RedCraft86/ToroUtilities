// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "IDetailGroup.h"
#include "DetailsPanel/ToroStructCustomization.h"
#include "DataTypes/SimpleCooldown.h"

#define STRUCT_NAME FSimpleCooldown
class FSimpleCooldownDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		FToroStructCustomization::CustomizeChildren(StructHandle, StructBuilder, StructCustomizationUtils);

		GET_STRUCT_PROPERTY_VAR_NS(Interval, Interval)
		ForwardMetadata(Interval);

		IDetailGroup& StructGroup = StructBuilder.AddGroup(TEXT("Cooldown"), StructHandle->GetPropertyDisplayName());
		StructGroup.HeaderRow().NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			Interval->CreatePropertyValueWidget()
		];

		StructGroup.AddPropertyRow(GET_STRUCT_PROPERTY_NS(Cooldown).ToSharedRef());
	}
};
#undef STRUCT_NAME