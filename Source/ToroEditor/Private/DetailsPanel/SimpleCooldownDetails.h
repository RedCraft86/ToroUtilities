// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"
#include "DataTypes/SimpleCooldown.h"

#define STRUCT_NAME FSimpleCooldown
class FSimpleCooldownDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		FToroStructCustomization::CustomizeChildren(StructHandle, StructBuilder, StructCustomizationUtils);

		IDetailGroup& StructGroup = StructBuilder.AddGroup(TEXT("Cooldown"), StructHandle->GetPropertyDisplayName());

		GET_STRUCT_PROPERTY_VAR_NS(Interval, Interval)

		ForwardMetadata(Interval);

		StructGroup.HeaderProperty(Interval.ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName());

		StructGroup.AddPropertyRow(GET_STRUCT_PROPERTY_NS(Cooldown).ToSharedRef());
	}
};
#undef STRUCT_NAME