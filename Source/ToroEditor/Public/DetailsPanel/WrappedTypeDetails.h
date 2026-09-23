// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

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