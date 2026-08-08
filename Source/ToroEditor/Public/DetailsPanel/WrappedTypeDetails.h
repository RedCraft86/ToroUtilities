// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "DataTypes/WrappedTypes.h"

#define STRUCT_NAME FWrappedBool
class FWrappedTypeDetails final : public FToroStructCustomization
{
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		FToroStructCustomization::CustomizeHeader(StructHandle, HeaderRow, CustomizationUtils);

		GET_STRUCT_PROPERTY_VAR_NS(Value, Value);

		ForwardMetadata(Value);

		// Display inner property as the struct itself
		HeaderRow.NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			Value->CreatePropertyValueWidget()
		];
	}
};
#undef STRUCT_NAME